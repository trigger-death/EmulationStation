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

GameDataList::GameDataList(sqlite3* db) :
	mDB(db), mRows(nullptr), mGame(nullptr), mMatchAll(false)
{
}

GameDataList::~GameDataList()
{
	delete mGame;
	resetQuery();
}

void GameDataList::filterSystem(std::string systemId)
{
	resetQuery();
	mSystemId = systemId;
}

void GameDataList::filterTags(std::set<std::string> tags, bool matchAll)
{
	resetQuery();
	mTags = tags;
	mMatchAll = matchAll;
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
		// See which type of query to build
		std::string query;
		if (mTags.size() > 0)
		{
			if (mMatchAll)
			{
				query = buildMatchAllQuery();
			}
			else
			{
				query = buildMatchAnyQuery();
			}
		}
		else
		{
			query = buildSimpleQuery();
		}
		sqlite3_prepare_v2(mDB, query.c_str(), query.size(), &mRows, NULL);
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
		mGame = new GameDataItem(mDB, (const char*)id, systemId ? (const char*)systemId : "",
								 path ? (const char*)path : "", rating, playCount);
		return mGame;
	}
	return nullptr;
}

void GameDataList::resetQuery()
{
	if (mRows)
	{
		sqlite3_finalize(mRows);
		mRows = nullptr;
	}
}

std::string GameDataList::buildMatchAnyQuery()
{
	// Build a query based on the system and tags
	std::stringstream query;
	query << "SELECT DISTINCT games.* FROM games INNER JOIN tags ON games.systemid=tags.systemid AND games.fileid=tags.fileid ";
	// See if there is a system clause
	if (!mSystemId.empty())
		query << "WHERE games.systemid='" << mSystemId << "' AND ";
	else
		query << "WHERE ";

	// Now add any tags
	bool first = true;
	for (auto it = mTags.begin(); it != mTags.end(); ++it)
	{
		if (!first)
			query << "OR ";
		first = false;
		query << "tags.tag='" << *it << "' ";
	}
	return query.str();
}

std::string GameDataList::buildMatchAllQuery()
{
	// Build a query based on the system and tags
	bool haveWhere = false;
	std::stringstream query;
	query << "SELECT DISTINCT games.* FROM games INNER JOIN tags ON games.systemid=tags.systemid AND games.fileid=tags.fileid ";
	// See if there is a system clause
	if (!mSystemId.empty())
	{
		query << "WHERE games.systemid='" << mSystemId << "' ";
		haveWhere = true;
	}
	// Now add any tags
	if (mTags.size() > 0)
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
		query << "(SELECT COUNT(*) FROM tags WHERE systemid=games.systemid AND fileid=games.fileid AND tag IN(";
	}
	bool first = true;
	for (auto it = mTags.begin(); it != mTags.end(); ++it)
	{
		if (first)
			query << "'" << *it << "' ";
		else
			query << ", '" << *it << "' ";
		first = false;
	}
	query << "))=" << mTags.size() << ";";
	return query.str();
}

std::string GameDataList::buildSimpleQuery()
{
	// Build a query based on the system
	std::stringstream query;
	query << "SELECT * FROM games ";
	// See if there is a system clause
	if (!mSystemId.empty())
	{
		query << "WHERE games.systemid='" << mSystemId << "' ";
	}
	query << ";";
	return query.str();
}
