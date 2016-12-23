/*
 * GameDataSystem.h
 *
 *  Created on: 18 Dec 2016
 *      Author: rhopkins
 */

#ifndef SRC_GAMEDATASYSTEM_H_
#define SRC_GAMEDATASYSTEM_H_

#include <string>
#include <boost/filesystem.hpp>
#include "PlatformId.h"

class GameDataSystem {
public:
	GameDataSystem(const std::string& id, const boost::filesystem::path& rootPath);
	virtual ~GameDataSystem();

	std::string id() const { return mID; }
	bool hasPlatformId(PlatformIds::PlatformId id) const;
	boost::filesystem::path getRootPath() const { return mRootPath; }

private:
	std::string				mID;
	boost::filesystem::path	mRootPath;
};

#endif /* SRC_GAMEDATASYSTEM_H_ */
