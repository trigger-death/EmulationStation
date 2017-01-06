/*
 * GameDataItem.h
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAITEM_H_
#define ES_GAMEDATA_SRC_GAMEDATAITEM_H_

#include <string>
#include <vector>

class GameDataFolder;

class GameDataItem
{
public:
	GameDataItem();
	virtual ~GameDataItem();

	/*!
	 * Get the display name for the item
	 *
	 * @return Display name
	 */
	virtual std::string name() = 0;

	/*!
	 * Get the parent folder of this folder.
	 *
	 * @return 	Parent or null if this is a root folder
	 */
	virtual GameDataFolder* parent();

	/*!
	 * Get the list of items in this folder
	 *
	 * @return List of items
	 */
	virtual const std::vector<GameDataItem*> items();
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAITEM_H_ */
