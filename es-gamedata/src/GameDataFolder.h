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
	 * Get or create a game folder in the database if it does not already exist
	 * @return Folder ID or an empty string if there is an error
	 * @param db			Database object
	 * @param systemId		System to create folder for
	 * @param rootPath		System root path. The game path must be the same as this or a
	 * 						subdirectory of this
	 * @param gamePath		Path to game including filename
	 * @param parent		Parent folder ID or an empty string if no parent
	 */
	static std::string getGameFolder(sqlite3* db, std::string systemId, std::string rootPath, std::string gamePath, std::string parent, bool create = true);

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
