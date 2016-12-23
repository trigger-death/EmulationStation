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
	TABLE_GAMES_FOLDER_COL
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
	 * Get the game list for a particular system
	 *
	 * @return			Gamelist
	 * @param system	System to get gamelist for
	 * @param flat		true=get a flat view of all the games (i.e. do not traverse folders)
	 * 					false=get top level view which may include folders
	 */
	GameDataList& getGameList(const GameDataSystem& system, bool flat);

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
