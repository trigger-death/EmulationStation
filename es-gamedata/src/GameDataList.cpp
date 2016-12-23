/*
 * GameDataList.cpp
 *
 *  Created on: 18 Dec 2016
 *      Author: rhopkins
 */

#include "GameDataList.h"
#include "GameDataItemFolder.h"
#include "GameDataSystem.h"
#include <sstream>
#include <iostream>

GameDataList::GameDataList(sqlite3* db, const GameDataSystem& system, bool flat) :
	mDB(db), mSystem(system), mFlat(flat)
{
	// Build a query to pass into the folder item we are creating
	std::stringstream ss_games;
	std::stringstream ss_folders;
	if (mFlat)
		ss_games << "SELECT * from games WHERE systemid='" << mSystem.id() << "';";
	else
	{
		// For a folder view we need to query both folders and games in the root directory
		ss_games << "SELECT * from games WHERE folder='';";
		ss_folders << "SELECT * from folders WHERE parent='';";
	}
	mRoot = new GameDataItemFolder(db, ss_games.str(), ss_folders.str());
}

GameDataList::~GameDataList()
{
	delete mRoot;
}

GameDataItemFolder* GameDataList::root()
{
	return mRoot;
}
