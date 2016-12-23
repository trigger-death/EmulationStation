/*
 * MockGameDatabase.h
 *
 *  Created on: 19 Dec 2016
 *      Author: rhopkins
 */

#ifndef ES_GAMEDATA_TEST_MOCKGAMEDATABASE_H_
#define ES_GAMEDATA_TEST_MOCKGAMEDATABASE_H_

#include <string>
#include <vector>
#include "sqlite3.h"

class MockGameDatabase {
public:
	MockGameDatabase(const std::string& path);
	virtual ~MockGameDatabase();

	void create();
	void close();

	void addGame(
		const std::string& 			fileid,
		const std::string& 			systemid,
		const std::string&			path,
		const std::string&			folder
	);
	void addMetadata(
		const std::string& 			fileid,
		const std::string& 			systemid,
		const std::string&			name,
		const std::string&			description,
		const std::string&			image,
		const std::string&			marquee,
		const std::string&			snapshot,
		const std::string&			thumbnail,
		const std::string&			video,
		const std::string&			releasedate,
		const std::string&			developer,
		const std::string&			publisher,
		const std::string&			genre,
		const std::string&			players
	);
	void addStats(
		const std::string& 			fileid,
		const std::string& 			systemid,
		const std::string&			rating,
		const std::string&			playcount,
		const std::string&			groups
	);
	void addFolder(
		const std::string&			name,
		const std::string&			description,
		const std::string&			image,
		const std::string&			thumbnail,
		const std::string&			parent
	);
	void addGroup(
		const std::string& 			name
	);

	void buildSql(const std::string& table, const std::vector<std::pair<std::string, std::string> >& fields, std::stringstream& sql);

	sqlite3*							mDB;
	std::string							mPath;
};

#endif /* ES_GAMEDATA_TEST_MOCKGAMEDATABASE_H_ */
