/*
 * GameData.h
 *
 *  Created on: 18 Dec 2016
 *      Author: rhopkins
 */

#ifndef SRC_GAMEDATA_H_
#define SRC_GAMEDATA_H_

#include <string>
#include <sstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <string>
#include "Log.h"
#include "GameDataSystem.h"
#include "GameDataList.h"
#include "sqlite3.h"

enum
{
	TABLE_GAMES_FILEID_COL,
	TABLE_GAMES_SYSTEMID_COL,
	TABLE_GAMES_PATH_COL,
	TABLE_GAMES_FOLDER_COL,
	TABLE_GAMES_TAGS_COL,
	TABLE_GAMES_RATING_COL,
	TABLE_GAMES_PLAYCOUNT_COL
};

enum
{
	TABLE_TAGS_SYSTEMID_COL,
	TABLE_TAGS_FILEID_COL,
	TABLE_TAGS_TAG_COL
};

enum
{
	TABLE_FOLDERS_ID_COL,
	TABLE_FOLDERS_SYSTEMID_COL,
	TABLE_FOLDERS_FULLPATH_COL,
	TABLE_FOLDERS_NAME_COL,
	TABLE_FOLDERS_DESCRIPTION_COL,
	TABLE_FOLDERS_IMAGE_COL,
	TABLE_FOLDERS_THUMBNAIL_COL,
	TABLE_FOLDERS_PARENT_COL
};

class GameData {
public:
	GameData();
	~GameData();

	/*!
	 * Get the singleton instance of this class
	 *
	 * @return Singleton instance
	 */
	static GameData& instance() { return mGameData; };

	/*!
	 * Open or create the database file
	 *
	 * @param path		Path to database file. The file is created if it doesn't already exist
	 */
	bool openDatabase(const boost::filesystem::path& path);

	/*!
	 * Parse a legacy gamelist.xml file
	 *
	 * @param system	System that the gamelist is for
	 * @param path		Path to the gamelist.xml to parse
	 */
	void parseGameList(const GameDataSystem& system, const boost::filesystem::path& path);

	/*!
	 * Create a game list that can be filtered
	 *
	 * @return			Gamelist. Must be deleted by the caller
	 */
	GameDataList* createGameList();

	/*!
	 * Populate the current game list with all games in the given directory
	 *
	 * @param system	System to populate
	 * @param rootPath	Root path for system
	 */
	void populateFolder(const GameDataSystem& system, std::string rootPath, std::vector<std::string> extensions);

	/*!
	 * Add a game to the gamelist if it doesn't already exist
	 *
	 * @param system	System to add game to
	 * @param path		Full path to file
	 * @param name		Optional name. If empty defaults to the filename without extension
	 */
	void addGame(const GameDataSystem& system, std::string path, std::string name = "");

	/*!
	 * Add metadata for a game if it doesn't already exist. If it does exist then it is left unchanged
	 * even if incomplete
	 *
	 * @param system	System to add metadata for
	 * @param id		Game id to add metadata for
	 * @param tag		Metadata tag to add
	 * @param value		Metadata value to add
	 */
	void addMetadata(const GameDataSystem& system, std::string id, std::string tag, std::string value);

protected:
	/*!
	 * Create the required set of tables if they don't already exist in the database
	 */
	void createTables();

	/*!
	 * Check to see if a game exists
	 *
	 * @return true if game exists in DB, false if it does not
	 * @param systemId	System id for game
	 * @param gameId	Game id
	 */
	bool gameExists(std::string systemId, std::string gameId);

	/*!
	 * Internal recursive function called by populateFolder()
	 *
	 * @param system	System to populate
	 * @param rootPath	Root path for system
	 */
	void populateFolderInternal(const GameDataSystem& system, std::string rootPath, std::vector<std::string> extensions);


protected:
	sqlite3*								mDB;
	bool									mIsOpen;
	std::map<std::string, GameDataList*>	mGames;

	static GameData							mGameData;
};



#endif /* SRC_GAMEDATA_H_ */
