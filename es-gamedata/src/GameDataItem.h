/*
 * GameDataItem.h
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAITEM_H_
#define ES_GAMEDATA_SRC_GAMEDATAITEM_H_

#include <string>

class GameDataItem
{
public:
	GameDataItem(const std::string& id, const std::string& systemID, const std::string& path,
				 const std::string& tags, int rating, int playCount);
	virtual ~GameDataItem();

	const std::string& id() const { return mID; }
	const std::string& systemID() const { return mSystemID; }
	const std::string& path() const { return mPath; }
	const std::string& tags() const { return mTags; }
	int	rating() const { return mRating; }
	int playCount() const { return mPlayCount; }

private:
	std::string		mID;
	std::string		mSystemID;
	std::string		mPath;
	std::string		mTags;
	int				mRating;
	int				mPlayCount;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAITEM_H_ */
