/*
 * GameDataFolder.h
 *
 *  Created on: 30 Dec 2016
 *      Author: rhopkins
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAFOLDER_H_
#define ES_GAMEDATA_SRC_GAMEDATAFOLDER_H_

#include "GameData.h"
#include "GameDataItem.h"
#include "GameDataGame.h"
#include <string>
#include <vector>
#include <map>
#include "sqlite3.h"

class GameDataItem;

class GameDataFolder : public GameDataItem
{
public:
	GameDataFolder(std::string name="");
	virtual ~GameDataFolder();

	/*!
	 * Get the display name for the item
	 *
	 * @return Display name
	 */
	virtual std::string name() { return mName; }

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

	/*!
	 * Add a game into the folder, building a hierarchy to match the game
	 * subdirectory if necessary
	 *
	 * @param game	Game to add. This class takes ownership of the game and
	 * 				deletes it when finished with
	 */
	void addGame(GameDataGame* game, boost::filesystem::path path = "");

private:
	std::vector<GameDataItem*>				mItems;
	std::map<std::string, GameDataFolder*> 	mFolders;
	std::string								mName;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAFOLDER_H_ */
