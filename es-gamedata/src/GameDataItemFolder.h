/*
 * GameDataItemFolder.h
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAITEMFOLDER_H_
#define ES_GAMEDATA_SRC_GAMEDATAITEMFOLDER_H_

#include <GameDataItem.h>
#include "GameDataSystem.h"
#include <string>
#include "sqlite3.h"

class GameDataItemGame;

class GameDataItemFolder: public GameDataItem {
public:
	GameDataItemFolder(sqlite3* db, bool flat, const GameDataSystem& system, const std::string& parentFolder);
	virtual ~GameDataItemFolder();

	virtual ITEM_TYPE getType() { return FOLDER; }

	virtual GameDataItem* firstChild();
	virtual GameDataItem* getNextChild();
	virtual GameDataItem* getChild(const std::string& id);

private:
	sqlite3*				mDB;
	bool					mFlat;
	const GameDataSystem& 	mSystem;
	const std::string&		mParentFolder;
	sqlite3_stmt*			mStmtGames;
	sqlite3_stmt*			mStmtFolders;
	GameDataItemGame*		mGame;
	GameDataItemFolder*		mFolder;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAITEMFOLDER_H_ */
