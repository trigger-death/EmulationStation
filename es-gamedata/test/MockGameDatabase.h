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
	MockGameDatabase(std::string path = ":memory:");
	virtual ~MockGameDatabase();

	void create();
	void close();

	void addGame(
		std::string 		fileid,
		std::string 		systemid,
		std::string			path,
		std::string			folder,
		std::string			rating,
		std::string			playcount
	);
	void addMetadata(
		std::string 		fileid,
		std::string 		systemid,
		std::string			name,
		std::string			description,
		std::string			image,
		std::string			marquee,
		std::string			snapshot,
		std::string			thumbnail,
		std::string			video,
		std::string			releasedate,
		std::string			developer,
		std::string			publisher,
		std::string			genre,
		std::string			players
	);
	void addFolder(
		std::string			id,
		std::string			systemid,
		std::string			fullpath,
		std::string			name,
		std::string			description,
		std::string			image,
		std::string			thumbnail,
		std::string			parent
	);
	void addTag(
		std::string 		fileid,
		std::string 		systemid,
		std::string 		tag
	);

	void buildSql(std::string table, const std::vector<std::pair<std::string, std::string> >& fields, std::stringstream& sql);

	sqlite3*							mDB;
	std::string							mPath;
};

#endif /* ES_GAMEDATA_TEST_MOCKGAMEDATABASE_H_ */
