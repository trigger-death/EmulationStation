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
#include <set>

class GameDataItem;

class GameDataList {
public:
	GameDataList(sqlite3* db);
	virtual ~GameDataList();

	//
	// Filter functions.
	// These must be called before getFirst() to configure what is returned in the list
	//

	/*!
	 * Filter the list based on the given system
	 *
	 * @param	systemId	System ID to filter on
	 */
	void filterSystem(std::string systemId);

	/*!
	 * Filter the list based on the given set of tags
	 *
	 * @param	tags		Tags to filter on
	 * @param	matchAll	true = all tags must match
	 * 						false = any tag can match
	 */
	void filterTags(std::set<std::string> tags, bool matchAll);

	virtual GameDataItem* getFirst();
	virtual GameDataItem* getNext();

private:
	/*!
	 * Clear out any existing query
	 */
	void resetQuery();

	/*!
	 * Build a query for the match-any case
	 */
	std::string buildMatchAnyQuery();

	/*!
	 * Build a query for the match-all case
	 */
	std::string buildMatchAllQuery();

	/*!
	 * Build a query that does not use tags
	 */
	std::string buildSimpleQuery();

private:
	sqlite3*					mDB;
	std::string					mSystemId;
	std::set<std::string>		mTags;
	bool						mMatchAll;
	sqlite3_stmt*				mRows;
	GameDataItem*				mGame;
};

#endif /* SRC_GAMEDATALIST_H_ */
