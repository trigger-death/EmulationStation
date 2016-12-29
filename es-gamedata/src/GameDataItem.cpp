/*
 * GameDataItem.cpp
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#include "GameDataItem.h"

GameDataItem::GameDataItem(sqlite3* db, const std::string& id, const std::string& systemID, const std::string& path,
						   int rating, int playCount ) :
	mID(id), mSystemID(systemID), mPath(path), mRating(rating), mPlayCount(playCount), mTags(db, systemID, id)
{
}

GameDataItem::~GameDataItem()
{
}

