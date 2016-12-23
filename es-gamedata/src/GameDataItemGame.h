/*
 * GameDataItemGame.h
 *
 *  Created on: 19 Dec 2016
 *      Author: roy
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAITEMGAME_H_
#define ES_GAMEDATA_SRC_GAMEDATAITEMGAME_H_

#include <GameDataItem.h>
#include <string>

class GameDataItemGame: public GameDataItem {
public:
	GameDataItemGame(const std::string& id, const std::string& systemID, const std::string& path);
	virtual ~GameDataItemGame();

	virtual ITEM_TYPE getType() { return GAME; }

	const std::string& id() const { return mID; }
	const std::string& systemID() const { return mSystemID; }
	const std::string& path() const { return mPath; }

private:
	std::string		mID;
	std::string		mSystemID;
	std::string		mPath;
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAITEMGAME_H_ */
