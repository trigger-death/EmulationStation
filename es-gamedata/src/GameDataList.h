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
class GameDataFolder;

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

	/*!
	 * Filter the list based on a system folder
	 *
	 * @param	folder		Folder to filter on
	 */
	void filterFolder(std::string folder);

	/*!
	 * Specify whether to flatten the folder structure into a single list or not
	 *
	 * @param	flat	true = flatten into a single folder
	 * 					false = maintain folder structure
	 */
	void filterFlat(bool flat);

	/*!
	 * Get the current system filter
	 *
	 * @return Filter folder or an empty string if not set
	 */
	std::string getFilterSystem();

	/*!
	 * Get the folder that represents the items in our query
	 *
	 * @return	Folder
	 */
	GameDataFolder* folder();

private:
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
	bool						mFlat;
	GameDataFolder*				mFolder;
};

#endif /* SRC_GAMEDATALIST_H_ */
