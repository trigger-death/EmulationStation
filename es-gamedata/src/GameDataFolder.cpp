/*
 * GameDataFolder.cpp
 *
 *  Created on: 30 Dec 2016
 *      Author: rhopkins
 */

#include "GameDataFolder.h"
#include "GameDataItem.h"
#include "GameDataException.h"
#include "Util.h"
#include <boost/filesystem.hpp>
#include <sstream>

namespace fs = boost::filesystem;

GameDataFolder::GameDataFolder(sqlite3* db, std::string gamesQuery, std::string folder) :
	mDB(db), mRows(nullptr), mGame(nullptr)
{
	// Build a query based on the given query but filtered on subfolder
	if (!folder.empty())
	{
		// Filter to show only games in the current folder. The games query folder field contains an ID
		// into the folders table
		std::stringstream query;
		query << "SELECT * FROM (" << gamesQuery << ") WHERE folder='" << folder << "' ;";
		mQuery = query.str();
	}
	else
	{
		// No folder filter. Just get all games in the games query
		mQuery = gamesQuery;
	}
}

GameDataFolder::~GameDataFolder()
{
	delete mGame;
	resetQuery();
}

GameDataFolder* GameDataFolder::parent()
{
	return nullptr;
}

GameDataFolder*	GameDataFolder::firstChild()
{
	return nullptr;
}

GameDataFolder* GameDataFolder::nextChild()
{
	return nullptr;
}

GameDataItem* GameDataFolder::getFirstItem()
{
	// See if we've already run the query
	if (mRows != nullptr)
	{
		// Rewind back to the beginning
		sqlite3_reset(mRows);
	}
	else
	{
		sqlite3_prepare_v2(mDB, mQuery.c_str(), mQuery.size(), &mRows, NULL);
	}
	// Just return the next item
	return getNextItem();
}

GameDataItem* GameDataFolder::getNextItem()
{
	// Find the first game
	if (mRows && (sqlite3_step(mRows) == SQLITE_ROW))
	{
		// Set our game item up to for this row
		delete mGame;
		const unsigned char* id = sqlite3_column_text(mRows, TABLE_GAMES_FILEID_COL);
		const unsigned char* systemId = sqlite3_column_text(mRows, TABLE_GAMES_SYSTEMID_COL);
		const unsigned char* path = sqlite3_column_text(mRows, TABLE_GAMES_PATH_COL);
		const unsigned char* tags = sqlite3_column_text(mRows, TABLE_GAMES_TAGS_COL);
		int rating = sqlite3_column_int(mRows, TABLE_GAMES_RATING_COL);
		int playCount = sqlite3_column_int(mRows, TABLE_GAMES_PLAYCOUNT_COL);
		mGame = new GameDataItem(mDB, (const char*)id, systemId ? (const char*)systemId : "",
								 path ? (const char*)path : "", rating, playCount);
		return mGame;
	}
	return nullptr;
}

void GameDataFolder::resetQuery()
{
	if (mRows)
	{
		sqlite3_finalize(mRows);
		mRows = nullptr;
	}
}

std::string GameDataFolder::getGameFolder(sqlite3* db, std::string systemId, std::string rootPath, std::string gamePath, std::string parent, bool create)
{
	std::string folderID;
	// Find the game path relative to the root path
	bool contains;
	// Find the relative path
	fs::path rel = removeCommonPath(gamePath, rootPath, contains);
	if (!contains)
	{
		LOG(LogError) << "Game path \"" << gamePath << "\" is outside system path \"" << rootPath << "\"";
		return nullptr;
	}
	// Remove the filename from the end
	rel = rel.remove_filename();

	// Now search the database to see if we already have this folder
	std::stringstream 	query;
	sqlite3_stmt*		qResult;
	query << "SELECT * FROM folders WHERE systemid='" << systemId << "' AND fullpath='" << rel.c_str() << "' ;";
	if (sqlite3_prepare_v2(db, query.str().c_str(), query.str().size(), &qResult, NULL) != SQLITE_OK)
		throw GameDataException("Failed to query folders table");
	if (sqlite3_step(qResult) == SQLITE_ROW)
	{
		// It's already there
		const unsigned char* id = sqlite3_column_text(qResult, TABLE_FOLDERS_ID_COL);
		if (id)
			folderID = (const char*)id;
		sqlite3_finalize(qResult);
	}
	else if (create)
	{
		// Not there. We need to create a new record
		sqlite3_finalize(qResult);
		fs::path name = rel.leaf();
		char* err;
		query.str("");
		query << "INSERT INTO folders ('systemid', 'fullpath', 'name', 'parent') VALUES ('" << systemId << "', '" << rel.c_str() <<
				  "', '" << name.c_str() << "', '" << parent << "')";
		if (sqlite3_exec(db, query.str().c_str(), NULL, NULL, &err) != SQLITE_OK)
		{
			std::stringstream error;
			error << "Failed to new folder: " << err;
			throw GameDataException(error.str());
		}
		// Call this function again to get the ID
		folderID = getGameFolder(db, systemId, rootPath, gamePath, parent, false);
	}

	return folderID;
}
