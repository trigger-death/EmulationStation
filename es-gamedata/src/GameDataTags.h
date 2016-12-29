/*
 * GameDataTags.h
 *
 *  Created on: 27 Dec 2016
 *      Author: rhopkins
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATATAGS_H_
#define ES_GAMEDATA_SRC_GAMEDATATAGS_H_

#include "sqlite3.h"
#include <string>
#include <set>

class GameDataTags {
public:
	GameDataTags(sqlite3* db, std::string systemid, std::string fileid);
	virtual ~GameDataTags();

	void addTag(std::string tag);
	void removeTag(std::string tag);
	bool hasTag(std::string tag);
	void setTags(std::set<std::string> tags);
	std::set<std::string> tags();

private:
	sqlite3* 					mDB;
	std::string					mSystemId;
	std::string					mFileId;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATATAGS_H_ */
