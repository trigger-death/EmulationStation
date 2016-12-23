/*
 * GameDataSystem.cpp
 *
 *  Created on: 18 Dec 2016
 *      Author: rhopkins
 */

#include <GameDataSystem.h>

GameDataSystem::GameDataSystem(const std::string& id, const boost::filesystem::path& rootPath) :
	mID(id), mRootPath(rootPath)
{
}

GameDataSystem::~GameDataSystem()
{
}

bool GameDataSystem::hasPlatformId(PlatformIds::PlatformId id) const
{
	if (mID == PlatformIds::getPlatformName(id))
		return true;
	return false;
}
