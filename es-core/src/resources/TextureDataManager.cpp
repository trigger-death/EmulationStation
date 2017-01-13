#include "resources/TextureDataManager.h"

TextureDataManager::TextureDataManager()
{
}

TextureDataManager::~TextureDataManager()
{
}

void TextureDataManager::add(TextureResource* key, std::shared_ptr<TextureData> data)
{
	mTextures[key] = data;
}

void TextureDataManager::remove(TextureResource* key)
{
	auto it = mTextures.find(key);
	if (it != mTextures.end())
		mTextures.erase(it);
}

std::shared_ptr<TextureData> TextureDataManager::get(TextureResource* key)
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
