/*
 * GameDataItem.h
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAITEM_H_
#define ES_GAMEDATA_SRC_GAMEDATAITEM_H_

#include <string>

class GameDataItem
{
public:
	// A GameDataItem can either represent a game or a folder. Only a
	// folder can have children
	enum ITEM_TYPE
	{
		GAME,
		FOLDER
	};

	GameDataItem();
	virtual ~GameDataItem();

	/*!
	 * Get the game data item type
	 *
	 * @return type
	 */
	virtual ITEM_TYPE getType() = 0;

	/*!
	 * Get the first child item
	 *
	 * @return Pointer to first child or NULL if no children
	 */
	virtual GameDataItem* firstChild()
	{
		return nullptr;
	}

	/*!
	 * Get the next child item
	 *
	 * @return Pointer to next child or NULL if no more children
	 */
	virtual GameDataItem* getNextChild()
	{
		return nullptr;
	}

	/*!
	 * Get a child item
	 *
	 * @return Game or NULL if the game doesn't exist or could not be created
	 * @param id		ID of game to get
	 * @param create	true=create game if it does not exist
	 * 					false=only return game if it currently exists
	 */
	virtual GameDataItem* getChild(const std::string& id, bool create)
	{
		return nullptr;
	}

};

#endif /* ES_GAMEDATA_SRC_GAMEDATAITEM_H_ */
