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

GameDataTags::GameDataTags(sqlite3* db, std::string systemid, std::string fileid) : mDB(db), mSystemId(systemid), mFileId(fileid)
{
}

GameDataTags::~GameDataTags()
{
}

void GameDataTags::addTag(std::string tag)
{
	// Check to see if the tag already exists
	std::set<std::string> current_tags = tags();
	if (current_tags.find(tag) == current_tags.end())
	{
		// Add it in
		char* err;
		std::stringstream query;
		query << "INSERT INTO tags ('systemid', 'fileid', 'tag') VALUES ('" << mSystemId << "', '" << mFileId << "', '" << tag << "') ;";
		if (sqlite3_exec(mDB, query.str().c_str(), NULL, NULL, &err) != SQLITE_OK)
		{
			std::stringstream error;
			error << "Failed to add tag for '" << mSystemId << "', '" << mFileId << "': " << err;
			throw GameDataException(error.str());
		}
	}
}

bool GameDataTags::hasTag(std::string tag)
{
	std::set<std::string> all_tags = tags();
	if (all_tags.find(tag) != all_tags.end())
		return true;
	return false;
}

void GameDataTags::removeTag(std::string tag)
{
	std::stringstream query;
	char* err;
	query << "DELETE FROM tags WHERE systemid='" << mSystemId << "' AND fileid='" << mFileId << "' AND tag='" << tag << "' ;";
	if (sqlite3_exec(mDB, query.str().c_str(), NULL, NULL, &err) != SQLITE_OK)
	{
		std::stringstream error;
		error << "Failed to delete tag for '" << mSystemId << "', '" << mFileId << "': " << err;
		throw GameDataException(error.str());
	}
}

void GameDataTags::setTags(std::set<std::string> tags)
{
	// Remove all existing tags
	std::stringstream query;
	char* err;
	query << "DELETE FROM tags WHERE systemid='" << mSystemId << "' AND fileid='" << mFileId << "' ;";
	if (sqlite3_exec(mDB, query.str().c_str(), NULL, NULL, &err) != SQLITE_OK)
	{
		std::stringstream error;
		error << "Failed to delete all tags for '" << mSystemId << "', '" << mFileId << "': " << err;
		throw GameDataException(error.str());
	}
	// Now add all new tags
	for (auto it = tags.begin(); it != tags.end(); ++it)
	{
		addTag(*it);
	}
}

std::set<std::string> GameDataTags::tags()
{
	sqlite3_stmt* 	qresult = nullptr;
	int				sqlResult;
	std::set<std::string>	result;

	// Build a query to get all tags for the game
	std::stringstream query;
	query << "SELECT * FROM tags WHERE systemid='" << mSystemId << "' AND fileid='" << mFileId << "' ;";
	sqlResult = sqlite3_prepare_v2(mDB, query.str().c_str(), query.str().size(), &qresult, NULL);
	if (sqlResult != SQLITE_OK)
	{
		std::stringstream error;
		error << "Failed to query tags database table. Error code: " << sqlResult;
		throw GameDataException(error.str());
	}

	// Iterate through the rows to get the tags
	while (sqlite3_step(qresult) == SQLITE_ROW)
	{
		const unsigned char* tag = sqlite3_column_text(qresult, TABLE_TAGS_TAG_COL);
		if (!tag)
			throw GameDataException("Invalid NULL tag found in tags table");
		result.insert((const char*)tag);
	}
	sqlite3_finalize(qresult);

	return result;
}

