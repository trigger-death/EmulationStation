/*
 * GameDataList.cpp
 *
 *  Created on: 18 Dec 2016
 *      Author: rhopkins
 */

#include "GameData.h"
#include "GameDataList.h"
#include "GameDataItem.h"
#include "GameDataFolder.h"
#include "GameDataException.h"
#include <sstream>
#include <iostream>

GameDataList::GameDataList(sqlite3* db) :
	mDB(db), mMatchAll(false), mFolder(nullptr), mFlat(true)
{
}

GameDataList::~GameDataList()
{
	delete mFolder;
}

void GameDataList::filterSystem(std::string systemId)
{
	mSystemId = systemId;
	delete mFolder;
	mFolder = nullptr;
}

std::string GameDataList::getFilterSystem()
{
	return mSystemId;
}

void GameDataList::filterTags(std::set<std::string> tags, bool matchAll)
{
	mTags = tags;
	mMatchAll = matchAll;
	delete mFolder;
	mFolder = nullptr;
}

void GameDataList::filterFlat(bool flat)
{
	mFlat = flat;
	delete mFolder;
	mFolder = nullptr;
}

std::string GameDataList::buildMatchAnyQuery()
{
	// Build a query based on the system and tags
	std::stringstream query;
	query << "SELECT DISTINCT games.*,metadata.value FROM games INNER JOIN tags ON games.systemid=tags.systemid AND games.fileid=tags.fileid ";
	query << "INNER JOIN metadata ON games.fileid=metadata.fileid AND games.systemid=metadata.systemid ";
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
	query << "AND metadata.tag='name' ";
	return query.str();
}

std::string GameDataList::buildMatchAllQuery()
{
	// Build a query based on the system and tags
	bool haveWhere = false;
	std::stringstream query;
	query << "SELECT DISTINCT games.*,metadata.value FROM games INNER JOIN tags ON games.systemid=tags.systemid AND games.fileid=tags.fileid ";
	query << "INNER JOIN metadata ON games.fileid=metadata.fileid AND games.systemid=metadata.systemid ";
	// See if there is a system clause
	if (!mSystemId.empty())
	{
		query << "WHERE games.systemid='" << mSystemId << "' AND metadata.tag='name' ";
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
	query << "))=" << mTags.size();
	return query.str();
}

std::string GameDataList::buildSimpleQuery()
{
	// Build a query based on the system
	std::stringstream query;
	query << "SELECT games.*,metadata.value FROM games INNER JOIN metadata ON games.fileid=metadata.fileid AND games.systemid=metadata.systemid ";
	// See if there is a system clause
	if (!mSystemId.empty())
	{
		query << "WHERE games.systemid='" << mSystemId << "' AND metadata.tag='name'";
	}
	return query.str();
}

GameDataFolder* GameDataList::folder()
{
	// Build the folder if not done already
	if (!mFolder)
	{
		// See which type of query to build
		std::string query;
		if (mTags.size() > 0)
		{
			if (mMatchAll)
				query = buildMatchAllQuery();
			else
				query = buildMatchAnyQuery();
		}
		else
			query = buildSimpleQuery();

		// We build the folder structure in memory based on the results of the query
		// rather than using the results as a recordset - this has less impact on the
		// existing ES code that worked on XML files and the gamelists are never that
		// huge
		sqlite3_stmt*	q_result;
		int result = sqlite3_prepare_v2(mDB, query.c_str(), query.size(), &q_result, NULL);
		if (result != SQLITE_OK)
		{
			std::stringstream error;
			error << "Failed to query game list folder using query: " << query << " - Error: " << error;
			throw GameDataException(error.str());
		}
		// Create an empty folder to start with
		mFolder = new GameDataFolder;

		// Iterate through the results and build the folder structure
		while (sqlite3_step(q_result) == SQLITE_ROW)
		{
			try
			{
				GameDataGame* game = new GameDataGame(mDB, q_result);

				// Add it to the folder. If the game path is in a subdirectory then the folder
				// may choose to build it into a folder hierarchy
				if (!mFlat)
					mFolder->addGame(game);
				else
				{
					// We are flattening the folder structure. Pass the game ID as the remaining
					// path - this ensures the game is added directly to the root folder
					mFolder->addGame(game, game->id());
				}
			}
			catch (GameDataException&)
			{
				// Just ignore the game and continue with the next one
			}
		}
	}
	return mFolder;
}
