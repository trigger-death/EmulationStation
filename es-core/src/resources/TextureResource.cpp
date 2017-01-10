#include "resources/TextureResource.h"
#include "Log.h"
#include "platform.h"
#include GLHEADER
#include "ImageIO.h"
#include "Renderer.h"
#include "Util.h"
#include "resources/SVGResource.h"
#include "Settings.h"

std::map< TextureResource::TextureKeyType, std::weak_ptr<TextureResource> > TextureResource::sTextureMap;
std::set<TextureResource*> 	TextureResource::sAllTextures;
TextureResourceCache	TextureResource::sCache;

TextureResource::TextureResource(const std::string& path, bool tile) : 
	mTextureID(0), mPath(path), mTextureSize(Eigen::Vector2i::Zero()), mTile(tile), mInitialized(false)
{
	sAllTextures.insert(this);
}

TextureResource::~TextureResource()
{
	sCache.remove(this);
	sAllTextures.erase(sAllTextures.find(this));
	deinit();
}

void TextureResource::unload(std::shared_ptr<ResourceManager>& rm)
{
	deinit();
}

void TextureResource::reload(std::shared_ptr<ResourceManager>& rm)
{
	if(!mPath.empty())
	{
		const ResourceData& data = rm->getFileData(mPath);
		initFromMemory((const char*)data.ptr.get(), data.length);

		// Mark it as used in the cache
		sCache.add(this);
	}
}

void TextureResource::initFromPixels(const unsigned char* dataRGBA, size_t width, size_t height)
{
	deinit();

	assert(width > 0 && height > 0);

	//now for the openGL texture stuff
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataRGBA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	const GLint wrapMode = mTile ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	mTextureSize << width, height;

	// The initialized flag remains true even if the texture is unloaded
	mInitialized = true;
}

void TextureResource::initFromMemory(const char* data, size_t length)
{
	size_t width, height;
	std::vector<unsigned char> imageRGBA = ImageIO::loadFromMemoryRGBA32((const unsigned char*)(data), length, width, height);

	if(imageRGBA.size() == 0)
	{
		LOG(LogError) << "Could not initialize texture from memory, invalid data!  (file path: " << mPath << ", data ptr: " << (size_t)data << ", reported size: " << length << ")";
		return;
	}

	// If we have a texture memory limit then make sure we have room for the next texture
	// by freeing the oldest item in the cache
	makeRoomForTexture(width, height);

	initFromPixels(imageRGBA.data(), width, height);
}

void TextureResource::deinit()
{
	if(mTextureID != 0)
	{
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}
}

const Eigen::Vector2i& TextureResource::getSize() const
{
	return mTextureSize;
}

bool TextureResource::isTiled() const
{
	return mTile;
}

void TextureResource::bind()
{
	if (mTextureID == 0)
	{
		// Attempt to reload this texture
		reload(ResourceManager::getInstance());
	}
	// Bind the texture
	if(mTextureID != 0)
		glBindTexture(GL_TEXTURE_2D, mTextureID);
	else
		LOG(LogError) << "Could not load and bind texture";
}

std::shared_ptr<TextureResource> TextureResource::get(const std::string& path, bool tile)
{
	std::shared_ptr<ResourceManager>& rm = ResourceManager::getInstance();

	const std::string canonicalPath = getCanonicalPath(path);

	if(canonicalPath.empty())
	{
		std::shared_ptr<TextureResource> tex(new TextureResource("", tile));
		rm->addReloadable(tex); //make sure we get properly deinitialized even though we do nothing on reinitialization
		return tex;
	}

	TextureKeyType key(canonicalPath, tile);
	auto foundTexture = sTextureMap.find(key);
	if(foundTexture != sTextureMap.end())
	{
		if(!foundTexture->second.expired())
			return foundTexture->second.lock();
	}

	// need to create it
	std::shared_ptr<TextureResource> tex;

	// is it an SVG?
	if(key.first.substr(key.first.size() - 4, std::string::npos) == ".svg")
	{
		// probably
		// don't add it to our map because 2 svgs might be rasterized at different sizes
		tex = std::shared_ptr<SVGResource>(new SVGResource(key.first, tile));
		rm->addReloadable(tex);
		tex->reload(rm);
		return tex;
	}else{
		// normal texture
		tex = std::shared_ptr<TextureResource>(new TextureResource(key.first, tile));
		sTextureMap[key] = std::weak_ptr<TextureResource>(tex);
		rm->addReloadable(tex);
		tex->reload(ResourceManager::getInstance());
		return tex;
	}
}

bool TextureResource::isInitialized() const
{
	return mInitialized;
}

size_t TextureResource::getMemUsage() const
{
	if(!mTextureID || mTextureSize.x() == 0 || mTextureSize.y() == 0)
		return 0;

	return mTextureSize.x() * mTextureSize.y() * 4;
}

size_t TextureResource::getTotalMemUsage()
{
	size_t size = 0;
	for (auto txt : sAllTextures)
	{
		if (txt->mTextureID != 0)
			size += txt->getMemUsage();
	}
	return size;
}

size_t TextureResource::getTotalTextureSize()
{
	size_t size = 0;
	for (auto txt : sAllTextures)
		size += txt->mTextureSize.x() * txt->mTextureSize.y() * 4;
	return size;
}

void TextureResource::makeRoomForTexture(int width, int height)
{
	size_t size = getTotalMemUsage();
	size_t tex_size = width * height * 4;
	size_t total = size + tex_size;
	TextureResource* purge = nullptr;

	size_t max_texture = (size_t)Settings::getInstance()->getInt("MaxVRAM") * 1024 * 1024;

	while ((total > max_texture) && ((purge = sCache.purgeOldest()) != nullptr))
	{
		// Just check it's still loaded
		if (purge->mTextureID != 0)
		{
			total -= purge->mTextureSize.x() * purge->mTextureSize.y() * 4;
			purge->unload(ResourceManager::getInstance());
		}
	}
}

TextureResourceCache::TextureResourceCache()
{
}

TextureResourceCache::~TextureResourceCache()
{
}

void TextureResourceCache::add(TextureResource* tex)
{
	// If it's in the cache then we want to remove it from it's current location
	remove(tex);
	// Add it to the front of the cache
	mCacheEntries.push_front(tex);
	mCacheLookup[tex] = mCacheEntries.begin();
}

void TextureResourceCache::remove(TextureResource* tex)
{
	// Find the entry in the list
	auto it = mCacheLookup.find(tex);
	if (it != mCacheLookup.end())
	{
		// Remove the list entry
		mCacheEntries.erase((*it).second);
		// And the lookup
		mCacheLookup.erase(it);
	}
}

TextureResource* TextureResourceCache::purgeOldest()
{
	// Pop the oldest one off the back
	TextureResource* tex = nullptr;
	if (mCacheEntries.size() > 0)
	{
		tex = mCacheEntries.back();
		mCacheEntries.pop_back();
		// Remove it from the lookup as well
		auto it = mCacheLookup.find(tex);
		if (it != mCacheLookup.end())
			mCacheLookup.erase(it);
	}
	return tex;
}
