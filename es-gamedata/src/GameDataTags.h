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
#include <vector>

class GameDataTags {
public:
	GameDataTags(sqlite3* db, std::string tagsField);
	GameDataTags(sqlite3* db, std::vector<std::string> tags);
	virtual ~GameDataTags();

	std::string tagField();
	std::vector<std::string> tags();

protected:
	/*!
	 * Get the tag ID for a text tag. Creates the entry in the tags database if
	 * it doesn't already exist
	 *
	 * @return 	tag ID
	 * @param 	tag		Textual tag to convert to ID
	 */
	std::string getTagID(std::string tag);

private:
	sqlite3* 					mDB;
	std::string					mTagsField;
	std::vector<std::string>	mTags;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATATAGS_H_ */
