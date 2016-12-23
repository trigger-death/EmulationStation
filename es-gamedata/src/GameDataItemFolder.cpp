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

GameDataItemFolder::GameDataItemFolder(sqlite3* db, const std::string& sqlGames, const std::string& sqlFolders) :
	mDB(db), mSqlGames(sqlGames), mSqlFolders(sqlFolders), mStmtGames(nullptr), mStmtFolders(nullptr), mGame(nullptr), mFolder(nullptr)
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
		if (!mSqlGames.empty())
			sqlite3_prepare_v2(mDB, mSqlGames.c_str(), mSqlGames.size(), &mStmtGames, NULL);
	}

	// See if the folders query has already run
	if (mStmtFolders != nullptr)
	{
		// Rewind back to the beginning
		sqlite3_reset(mStmtFolders);
	}
	else
	{
		if (!mSqlFolders.empty())
			sqlite3_prepare_v2(mDB, mSqlFolders.c_str(), mSqlFolders.size(), &mStmtFolders, NULL);
	}
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
		mFolder = new GameDataItemFolder(mDB, mSqlGames, mSqlFolders);
		return mFolder;
	}
	return nullptr;
}

GameDataItem* GameDataItemFolder::getNextChild()
{
	return nullptr;
}

GameDataItem* GameDataItemFolder::getChild(const std::string& id)
{
	return nullptr;
}
