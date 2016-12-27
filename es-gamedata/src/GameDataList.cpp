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
	// Build the root folder
	mRoot = new GameDataItemFolder(db, flat, system, "");
}

GameDataList::~GameDataList()
{
	delete mRoot;
}

GameDataItemFolder* GameDataList::root()
{
	return mRoot;
}
