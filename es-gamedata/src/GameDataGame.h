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
#include "GameDataMetadata.h"
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

	/*!
	 * Get the display name for the item
	 *
	 * @return Display name
	 */
	virtual std::string name() { return mName; };

	const std::string& id() const { return mID; }
	const std::string& systemID() const { return mSystemID; }
	const std::string& path() const { return mPath; }
	GameDataTags& tags() { return *mTags; }
	int	rating() const { return mRating; }
	int playCount() const { return mPlayCount; }
	GameDataMetadata& metadata() { return *mMetadata; }

private:
	std::string		mID;
	std::string		mSystemID;
	std::string		mPath;
	int				mRating;
	int				mPlayCount;
	std::string		mName;
	GameDataTags*	mTags;
	GameDataMetadata* mMetadata;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAGAME_H_ */
