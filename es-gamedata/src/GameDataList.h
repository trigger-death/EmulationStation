/*
 * GameDataList.h
 *
 *  Created on: 18 Dec 2016
 *      Author: rhopkins
 */

#ifndef SRC_GAMEDATALIST_H_
#define SRC_GAMEDATALIST_H_

#include "sqlite3.h"
#include <string>
#include <vector>

class GameDataItem;

class GameDataList {
public:
	GameDataList(sqlite3* db, std::string system, std::vector<std::string> tags);
	virtual ~GameDataList();

	virtual GameDataItem* getFirst();
	virtual GameDataItem* getNext();

private:
	sqlite3*					mDB;
	std::string					mSystem;
	std::vector<std::string>	mTags;
	sqlite3_stmt*				mRows;
	GameDataItem*				mGame;
};

#endif /* SRC_GAMEDATALIST_H_ */
