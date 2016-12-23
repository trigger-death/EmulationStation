/*
 * GameDataList.h
 *
 *  Created on: 18 Dec 2016
 *      Author: rhopkins
 */

#ifndef SRC_GAMEDATALIST_H_
#define SRC_GAMEDATALIST_H_

#include "sqlite3.h"
class GameDataItemFolder;
class GameDataSystem;

class GameDataList {
public:
	GameDataList(sqlite3* db, const GameDataSystem& system, bool flat);
	virtual ~GameDataList();

	/*!
	 * Get the root item. this will be a 'folder' type item that contains child items
	 *
	 * @return Pointer to root item
	 */
	GameDataItemFolder* root();

private:
	sqlite3*					mDB;
	const GameDataSystem&		mSystem;
	bool						mFlat;
	GameDataItemFolder*			mRoot;
};

#endif /* SRC_GAMEDATALIST_H_ */
