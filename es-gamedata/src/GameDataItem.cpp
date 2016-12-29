/*
 * GameDataItem.cpp
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#include "GameDataItem.h"

GameDataItem::GameDataItem(const std::string& id, const std::string& systemID, const std::string& path,
						   const std::string& tags, int rating, int playCount ) :
	mID(id), mSystemID(systemID), mPath(path), mTags(tags), mRating(rating), mPlayCount(playCount)
{
}

GameDataItem::~GameDataItem()
{
}

