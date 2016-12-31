/*
 * GameDataFolder.h
 *
 *  Created on: 30 Dec 2016
 *      Author: rhopkins
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAFOLDER_H_
#define ES_GAMEDATA_SRC_GAMEDATAFOLDER_H_

#include "GameData.h"
#include <string>
#include "sqlite3.h"

class GameDataItem;

class GameDataFolder {
public:
	GameDataFolder(sqlite3* db, std::string gamesQuery, std::string folder);
	virtual ~GameDataFolder();

	std::string	systemId() { return mSystemId; }
	std::string fullPath() { return mPath; }
	std::string name() { return mName; }
	std::string description() { return mDescription; }
	std::string image() { return mImage; }
	std::string thumbnail() { return mThumbnail; }

	/*!
	 * Get the parent folder of this folder.
	 *
	 * @return 	Parent or null if this is a root folder
	 */
	GameDataFolder* parent();

	/*!
	 * Get the first child folder
	 *
	 * @return First child or null if no children
	 */
	GameDataFolder*	firstChild();

	/*!
	 * Get the next child folder
	 *
	 * @return Next child or null if no more children
	 */
	GameDataFolder* nextChild();

	/*!
	 * Get the first game item in this folder
	 *
	 * @return First game item or null if no items
	 */
	GameDataItem* getFirstItem();

	/*!
	 * Get the next game item in this folder
	 *
	 * @return Next game item or null if no more items
	 */
	GameDataItem* getNextItem();

private:
	/*!
	 * Clear out any existing query results
	 */
	void resetQuery();

private:
	sqlite3* 					mDB;
	sqlite3_stmt*				mRows;
	GameDataItem*				mGame;
	std::string					mQuery;
	std::string					mSystemId;
	std::string					mPath;
	std::string					mName;
	std::string					mDescription;
	std::string					mImage;
	std::string					mThumbnail;

};

#endif /* ES_GAMEDATA_SRC_GAMEDATAFOLDER_H_ */
