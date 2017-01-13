#pragma once

#include "resources/ResourceManager.h"
#include "platform.h"
#include "resources/TextureData.h"
#include <map>
#include <memory>

class TextureResource;

//
// This class manages the loading and unloading of textures
//
// When textures are added, the texture data is just stored as-is. The texture
// data should only have been constructed and not loaded for this to work correctly.
// When the get() function is called it indicates that a texture wants to be used so
// at this point the texture data is loaded (via a call to load()).
//
// Once the load is complete (which may not be on the first call to get() if the
// data is loaded in a background thread) then the get() function call uploadAndBind()
// to upload to VRAM if necessary and bind the texture. This is followed by a call
// to releaseRAM() which frees the memory buffer if the texture can be reloaded from
// disk if needed again
//
class TextureDataManager
{
public:
	TextureDataManager();
	~TextureDataManager();

	std::shared_ptr<TextureData> add(const TextureResource* key, bool tiled);

	// The texturedata being removed may be loading in a different thread. However it will
	// be referenced by a smart point so we only need to remove it from our array and it
	// will be deleted when the other thread has finished with it
	void remove(const TextureResource* key);

	std::shared_ptr<TextureData> get(const TextureResource* key);

private:
	std::map<const TextureResource*, std::shared_ptr<TextureData> > mTextures;
};

