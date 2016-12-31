/*
 * GameDataFolder.cpp
 *
 *  Created on: 30 Dec 2016
 *      Author: rhopkins
 */

#include "GameDataFolder.h"
#include "GameDataItem.h"
#include <sstream>

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
