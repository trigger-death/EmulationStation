/*
 * GameDataItemFolder.h
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAITEMFOLDER_H_
#define ES_GAMEDATA_SRC_GAMEDATAITEMFOLDER_H_

#include <GameDataItem.h>
#include <string>
#include "sqlite3.h"

class GameDataItemGame;

class GameDataItemFolder: public GameDataItem {
public:
	GameDataItemFolder(sqlite3* db, const std::string& sqlGames, const std::string& sqlFolders);
	virtual ~GameDataItemFolder();

	virtual ITEM_TYPE getType() { return FOLDER; }

	virtual GameDataItem* firstChild();
	virtual GameDataItem* getNextChild();
	virtual GameDataItem* getChild(const std::string& id);

private:
	sqlite3*			mDB;
	sqlite3_stmt*		mStmtGames;
	sqlite3_stmt*		mStmtFolders;
	std::string			mSqlGames;
	std::string			mSqlFolders;
	GameDataItemGame*	mGame;
	GameDataItemFolder*	mFolder;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAITEMFOLDER_H_ */
