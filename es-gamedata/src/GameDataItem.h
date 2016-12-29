/*
 * GameDataItem.h
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAITEM_H_
#define ES_GAMEDATA_SRC_GAMEDATAITEM_H_

#include <string>
#include "GameDataTags.h"
#include "sqlite3.h"

class GameDataItem
{
public:
	GameDataItem(sqlite3* db, const std::string& id, const std::string& systemID, const std::string& path,
				 int rating, int playCount);
	virtual ~GameDataItem();

	const std::string& id() const { return mID; }
	const std::string& systemID() const { return mSystemID; }
	const std::string& path() const { return mPath; }
	GameDataTags& tags() { return mTags; }
	int	rating() const { return mRating; }
	int playCount() const { return mPlayCount; }

private:
	std::string		mID;
	std::string		mSystemID;
	std::string		mPath;
	int				mRating;
	int				mPlayCount;
	GameDataTags	mTags;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAITEM_H_ */
