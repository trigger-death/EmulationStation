/*
 * GameDataTags.cpp
 *
 *  Created on: 27 Dec 2016
 *      Author: rhopkins
 */

#include "GameData.h"
#include "GameDataTags.h"
#include "GameDataException.h"
#include <sstream>

GameDataTags::GameDataTags(sqlite3* db, std::string tagsField) : mDB(db), mTagsField(tagsField)
{
}

GameDataTags::GameDataTags(sqlite3* db, std::vector<std::string> tags) : mDB(db), mTags(tags)
{
}

GameDataTags::~GameDataTags()
{
}

std::string GameDataTags::tagField()
{
	if (mTagsField.empty() && !mTags.empty())
	{
		// Convert from a list of text tags to a comma separated list of tag IDs
	}
	return mTagsField;
}

std::vector<std::string> GameDataTags::tags()
{
	if (mTags.empty() && !mTagsField.empty())
	{
		// Convert from a comma separated list of tag IDs to a list of text tags
	}
	return mTags;
}

std::string GameDataTags::getTagID(std::string tag)
{
	sqlite3_stmt* 	qresult = nullptr;
	std::string 	result;
	int				sqlResult;
	// See if the tag already exists
	std::stringstream query;
	query << "SELECT * FROM tags WHERE tag='" << tag << "';";
	sqlResult = sqlite3_prepare_v2(mDB, query.str().c_str(), query.str().size(), &qresult, NULL);
	if (sqlResult != SQLITE_OK)
	{
		std::stringstream error;
		error << "Failed to query tags database table. Error code: " << sqlResult;
		throw GameDataException(error.str());
	}
	// See if there is a row of data
	if (sqlite3_step(qresult) == SQLITE_ROW)
	{
		std::stringstream id;
		id << sqlite3_column_int(qresult, TABLE_TAGS_ID_COL);
		result = id.str();
	}
	sqlite3_finalize(qresult);

	// See if it previously existed
	if (result.empty())
	{
		// Doesn't exist. Add a new row
		std::stringstream sql;
		sql << "INSERT INTO tags (tag) VALUES ('" << tag << "');";
		sqlResult = sqlite3_exec(mDB, sql.str().c_str(), NULL, NULL, NULL);
		if (sqlResult != SQLITE_OK)
		{
			std::stringstream error;
			error << "Failed to add tag database table. Error code: " << sqlResult;
			throw GameDataException(error.str());
		}
		// Now we can just recursively call this function to get the ID
		return getTagID(tag);
	}
	return result;
}
