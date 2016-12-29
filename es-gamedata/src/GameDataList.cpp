/*
 * GameDataList.cpp
 *
 *  Created on: 18 Dec 2016
 *      Author: rhopkins
 */

#include "GameData.h"
#include "GameDataList.h"
#include "GameDataItem.h"
#include <sstream>
#include <iostream>

GameDataList::GameDataList(sqlite3* db, std::string system, std::vector<std::string> tags) :
	mDB(db), mSystem(system), mTags(tags), mRows(nullptr), mGame(nullptr)
{
}

GameDataList::~GameDataList()
{
	delete mGame;
	if (mRows)
		sqlite3_finalize(mRows);
}

GameDataItem* GameDataList::getFirst()
{
	// See if we've already run the query
	if (mRows != nullptr)
	{
		// Rewind back to the beginning
		sqlite3_reset(mRows);
	}
	else
	{
		// Build a query based on the system and tags
		bool haveWhere = false;
		std::stringstream query;
		query << "SELECT * FROM games ";
		// See if there is a system clause
		if (!mSystem.empty())
		{
			query << "WHERE systemid='" << mSystem << "' ";
			haveWhere = true;
		}
		// Now add any tags
		for (auto it = mTags.begin(); it != mTags.end(); ++it)
		{
			// Make sure we've added the 'WHERE' tag
			if (!haveWhere)
			{
				query << "WHERE ";
				haveWhere = true;
			}
			else
			{
				query << "AND ";
			}
			query << "where (',' || tags || ',') LIKE '%," << *it << ",%') ";
		}
		sqlite3_prepare_v2(mDB, query.str().c_str(), query.str().size(), &mRows, NULL);
	}
	// Just return the next item
	return getNext();
}

GameDataItem* GameDataList::getNext()
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
		mGame = new GameDataItem((const char*)id, systemId ? (const char*)systemId : "",
								 path ? (const char*)path : "", tags ? (const char*)tags : "",
								 rating, playCount);
		return mGame;
	}
	return nullptr;
}
