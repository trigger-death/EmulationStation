/*
 * GameDataItem.cpp
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#include "GameDataItem.h"

GameDataItem::GameDataItem()
{
}

GameDataItem::~GameDataItem()
{
}

/*!
 * Get the parent folder of this folder.
 *
 * @return 	Parent or null if this is a root folder
 */
GameDataFolder* GameDataItem::parent()
{
	return nullptr;
}

/*!
 * Get the list of items in this folder
 *
 * @return List of items
 */
const std::vector<GameDataItem*> GameDataItem::items()
{
	std::vector<GameDataItem*> empty;
	return empty;
}
