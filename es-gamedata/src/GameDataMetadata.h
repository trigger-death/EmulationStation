/*
 * GameDataMetadata.h
 *
 *  Created on: 7 Jan 2017
 *      Author: rhopkins
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAMETADATA_H_
#define ES_GAMEDATA_SRC_GAMEDATAMETADATA_H_

#include <string>
#include <map>
#include "sqlite3.h"

class GameDataMetadata {
public:
	GameDataMetadata(sqlite3* db, const std::string& systemID, const std::string& id);
	virtual ~GameDataMetadata();

	/*!
	 * Read the metadata from the database and return it as a set of tag/value pairs
	 *
	 * @return Tag/value pairs
	 */
	std::map<std::string, std::string> data();

private:
	sqlite3*		mDB;
	std::string		mID;
	std::string		mSystemID;
	std::map<std::string, std::string> mData;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAMETADATA_H_ */
