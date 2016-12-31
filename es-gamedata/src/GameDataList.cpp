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
#include <sstream>
#include <iostream>

GameDataList::GameDataList(sqlite3* db) :
	mDB(db), mMatchAll(false), mFolder(nullptr)
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

void GameDataList::filterTags(std::set<std::string> tags, bool matchAll)
{
	mTags = tags;
	mMatchAll = matchAll;
	delete mFolder;
	mFolder = nullptr;
}

void GameDataList::filterFolder(std::string folder)
{
	mFolderFilter = folder;
	delete mFolder;
	mFolder = nullptr;
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
	query << "))=" << mTags.size();
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
		// Pass the query to the folder along with the folder filter
		mFolder = new GameDataFolder(mDB, query, mFolderFilter);
	}
	return mFolder;
}
