/*
 * GameDataItemGame.cpp
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#include <GameDataItemGame.h>

GameDataItemGame::GameDataItemGame(const std::string& id, const std::string& systemID, const std::string& path) :
	mID(id), mSystemID(systemID), mPath(path)
{
}

GameDataItemGame::~GameDataItemGame()
{
}

