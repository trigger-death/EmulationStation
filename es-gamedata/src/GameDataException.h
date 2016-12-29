/*
 * GameDataException.h
 *
 *  Created on: 27 Dec 2016
 *      Author: rhopkins
 */

#ifndef ES_GAMEDATA_SRC_GAMEDATAEXCEPTION_H_
#define ES_GAMEDATA_SRC_GAMEDATAEXCEPTION_H_

#include <string>

class GameDataException {
public:
	GameDataException(std::string message);
	virtual ~GameDataException();
};

#endif /* ES_GAMEDATA_SRC_GAMEDATAEXCEPTION_H_ */
