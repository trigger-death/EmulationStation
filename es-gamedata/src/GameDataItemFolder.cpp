/*
 * GameDataItemFolder.cpp
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#include "GameData.h"
#include <GameDataItemFolder.h>
#include <GameDataItemGame.h>
#include "sqlite3.h"

GameDataItemFolder::GameDataItemFolder(sqlite3* db, bool flat, const GameDataSystem& system, const std::string& parentFolder) :
	mDB(db), mFlat(flat), mSystem(system), mParentFolder(parentFolder), mStmtGames(nullptr), mStmtFolders(nullptr), mGame(nullptr), mFolder(nullptr)
{
}

GameDataItemFolder::~GameDataItemFolder()
{
	delete mGame;
	delete mFolder;
	if (mStmtGames)
		sqlite3_finalize(mStmtGames);
	if (mStmtFolders)
		sqlite3_finalize(mStmtFolders);
}

GameDataItem* GameDataItemFolder::firstChild()
{
	// See if the games query has already run
	if (mStmtGames != nullptr)
	{
		// Rewind back to the beginning
		sqlite3_reset(mStmtGames);
	}
	else
	{
		// Build a query for the games
		std::stringstream ss_games;
		if (mFlat)
			ss_games << "SELECT * from games WHERE systemid='" << mSystem.id() << "';";
		else
			ss_games << "SELECT * from games WHERE systemid='" << mSystem.id() << "' AND folder='';";
		sqlite3_prepare_v2(mDB, ss_games.str().c_str(), ss_games.str().size(), &mStmtGames, NULL);
	}

	// See if the folders query has already run
	if (mStmtFolders != nullptr)
	{
		// Rewind back to the beginning
		sqlite3_reset(mStmtFolders);
	}
	else
	{
		// Build a query for the folders
		std::stringstream ss_folders;
		if (!mFlat)
		{
			ss_folders << "SELECT * from folders WHERE systemid='" << mSystem.id() << "' AND parent='';";
			sqlite3_prepare_v2(mDB, ss_folders.str().c_str(), ss_folders.str().size(), &mStmtFolders, NULL);
		}
	}
	// Just return the next child
	return getNextChild();
}

GameDataItem* GameDataItemFolder::getNextChild()
{
	// Find the first game
	if (mStmtGames && (sqlite3_step(mStmtGames) == SQLITE_ROW))
	{
		// Set our game item up to for this row
		delete mGame;
		std::string id((const char*)sqlite3_column_text(mStmtGames, TABLE_GAMES_FILEID_COL));
		std::string systemId((const char*)sqlite3_column_text(mStmtGames, TABLE_GAMES_SYSTEMID_COL));
		std::string path((const char*)sqlite3_column_text(mStmtGames, TABLE_GAMES_PATH_COL));
		mGame = new GameDataItemGame(id, systemId, path);
		return mGame;
	}
	else if (mStmtFolders && (sqlite3_step(mStmtFolders) == SQLITE_ROW))
	{
		delete mFolder;
		std::string folder((const char*)sqlite3_column_text(mStmtFolders,0));
		mFolder = new GameDataItemFolder(mDB, false, mSystem, folder);
		return mFolder;
	}
	return nullptr;
}

GameDataItem* GameDataItemFolder::getChild(const std::string& id)
{
	return nullptr;
}
