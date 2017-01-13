#include "resources/TextureDataManager.h"

TextureDataManager::TextureDataManager()
{
}

TextureDataManager::~TextureDataManager()
{
}

std::shared_ptr<TextureData> TextureDataManager::add(const TextureResource* key, bool tiled)
{
	std::shared_ptr<TextureData> data(new TextureData(tiled));
	mTextures[key] = data;
	return data;
}

void TextureDataManager::remove(const TextureResource* key)
{
	auto it = mTextures.find(key);
	if (it != mTextures.end())
		mTextures.erase(it);
}

std::shared_ptr<TextureData> TextureDataManager::get(const TextureResource* key)
{
	std::shared_ptr<TextureData> tex;

	auto it = mTextures.find(key);
	if (it != mTextures.end())
	{
		tex = (*it).second;
		tex->load();
	}

	return tex;
}
