/*
 * GameDataGame.h
 *
 *  Created on: 5 Jan 2017
 *      Author: rhopkins
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAGAME_H_
#define ES_GAMEDATA_SRC_GAMEDATAGAME_H_

#include <string>
#include "GameDataTags.h"
#include "GameDataItem.h"
#include "sqlite3.h"

class GameDataGame: public GameDataItem {
public:
	/*!
	 * Constructor from individual fields
	 */
	GameDataGame(sqlite3* db, const std::string& id, const std::string& systemID, const std::string& path,
				 int rating, int playCount);

	/*!
	 * Constructor from database row
	 */
	GameDataGame(sqlite3* db, sqlite3_stmt* row);

	virtual ~GameDataGame();

	const std::string& id() const { return mID; }
	const std::string& systemID() const { return mSystemID; }
	const std::string& path() const { return mPath; }
	GameDataTags& tags() { return *mTags; }
	int	rating() const { return mRating; }
	int playCount() const { return mPlayCount; }

private:
	std::string		mID;
	std::string		mSystemID;
	std::string		mPath;
	int				mRating;
	int				mPlayCount;
	GameDataTags*	mTags;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAGAME_H_ */
