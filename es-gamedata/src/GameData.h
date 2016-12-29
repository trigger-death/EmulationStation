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

//
// Temporary log code
//
enum LogLevel { LogError, LogWarning, LogInfo, LogDebug };
#define LOG(level) std::cerr

class GameData {
public:
	GameData();
	~GameData();

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
	 * Get a game list that can be filtered
	 *
	 * @return			Gamelist
	 */
	GameDataList getGameList();

protected:
	/*!
	 * Create the required set of tables if they don't already exist in the database
	 */
	void createTables();

protected:
	sqlite3*								mDB;
	bool									mIsOpen;
	std::map<std::string, GameDataList*>	mGames;
};



#endif /* SRC_GAMEDATA_H_ */
