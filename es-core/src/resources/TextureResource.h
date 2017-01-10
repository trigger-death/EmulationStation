#pragma once

#include "resources/ResourceManager.h"

#include <string>
#include <set>
#include <list>
#include <Eigen/Dense>
#include "platform.h"
#include GLHEADER

class TextureResource;

class TextureResourceCache
{
public:
	TextureResourceCache();
	~TextureResourceCache();

	void add(TextureResource* tex);
	void remove(TextureResource* tex);
	TextureResource* purgeOldest();

private:
	std::list<TextureResource*>											mCacheEntries;
	std::map<TextureResource*, std::list<TextureResource*>::iterator> 	mCacheLookup;
};

// An OpenGL texture.
// Automatically recreates the texture with renderer deinit/reinit.
class TextureResource : public IReloadable
{
public:
	static std::shared_ptr<TextureResource> get(const std::string& path, bool tile = false);

	virtual ~TextureResource();

	virtual void unload(std::shared_ptr<ResourceManager>& rm) override;
	virtual void reload(std::shared_ptr<ResourceManager>& rm) override;
	
	bool isInitialized() const;
	bool isTiled() const;
	const Eigen::Vector2i& getSize() const;
	void bind();
	
	// Warning: will NOT correctly reinitialize when this texture is reloaded (e.g. ES starts/stops playing a game).
	virtual void initFromMemory(const char* file, size_t length);

	// Warning: will NOT correctly reinitialize when this texture is reloaded (e.g. ES starts/stops playing a game).
	void initFromPixels(const unsigned char* dataRGBA, size_t width, size_t height);

	size_t getMemUsage() const; // returns an approximation of the VRAM used by this texture (in bytes)
	static size_t getTotalMemUsage(); // returns an approximation of total VRAM used by textures (in bytes)
	static size_t getTotalTextureSize(); // returns the number of bytes that would be used if all textures were in memory

protected:
	TextureResource(const std::string& path, bool tile);
	void deinit();

	void makeRoomForTexture(int width, int height);

	Eigen::Vector2i mTextureSize;
	const std::string mPath;
	const bool mTile;
	bool	mInitialized;

private:
	GLuint mTextureID;

	typedef std::pair<std::string, bool> TextureKeyType;
	static std::map< TextureKeyType, std::weak_ptr<TextureResource> > sTextureMap; // map of textures, used to prevent duplicate textures
	static std::set<TextureResource*> 	sAllTextures;	// Set of all textures, used for memory management
	static TextureResourceCache			sCache;
};
