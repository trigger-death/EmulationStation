/*
 * MockGameDatabase.cpp
 *
 *  Created on: 19 Dec 2016
 *      Author: rhopkins
 */

#include "MockGameDatabase.h"
#include <gtest/gtest.h>

MockGameDatabase::MockGameDatabase(const std::string& path) : mDB(nullptr), mPath(path)
{
}

MockGameDatabase::~MockGameDatabase()
{
}

void MockGameDatabase::create()
{
	int result = sqlite3_open_v2(mPath.c_str(), &mDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	ASSERT_EQ(result, SQLITE_OK);
	std::stringstream ss;

	// Metadata
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS metadata (" <<
		"fileid VARCHAR(255) NOT NULL, " <<
		"systemid VARCHAR(255) NOT NULL, " <<
		"name TEXT, " <<
		"description TEXT, " <<
		"image TEXT, " <<
		"marquee TEXT, " <<
		"snapshot TEXT, " <<
		"thumbnail TEXT, " <<
		"video TEXT, " <<
		"releasedate TEXT, " <<
		"developer TEXT, " <<
		"publisher TEXT, " <<
		"genre TEXT, " <<
		"players INT DEFAULT 1, " <<
		"PRIMARY KEY (fileid, systemid))";
	ASSERT_EQ(sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL), SQLITE_OK);

	// Stats
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS stats (" <<
		"fileid VARCHAR(255) NOT NULL, " <<
		"systemid VARCHAR(255) NOT NULL, " <<
		"rating INT DEFAULT 3, " <<
		"playcount INT DEFAULT 0, " <<
		"groups TEXT, " <<
		"PRIMARY KEY (fileid, systemid))";
	ASSERT_EQ(sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL), SQLITE_OK);

	// Games
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS games (" <<
		"fileid VARCHAR(255) NOT NULL, " <<
		"systemid VARCHAR(255) NOT NULL, " <<
		"path TEXT, " <<
		"folder TEXT, " <<
		"PRIMARY KEY (fileid, systemid))";
	ASSERT_EQ(sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL), SQLITE_OK);

	// Folders
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS folders (" <<
		"path TEXT NOT NULL, " <<
		"description TEXT, " <<
		"image TEXT, " <<
		"thumbnail TEXT, " <<
		"PRIMARY KEY (path))";
	ASSERT_EQ(sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL), SQLITE_OK);

	// Groups
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS groups (" <<
		"name TEXT NOT NULL, " <<
		"PRIMARY KEY (name))";
	ASSERT_EQ(sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL), SQLITE_OK);
}

void MockGameDatabase::close()
{
	if (mDB)
		sqlite3_close(mDB);
	mDB = nullptr;
}

void MockGameDatabase::buildSql(const std::string& table, const std::vector<std::pair<std::string, std::string> >& fields, std::stringstream& sql)
{
	bool first = true;
	sql << "INSERT INTO " << table << " ('";
	for (auto it = fields.begin(); it != fields.end(); ++it)
	{
		if (!first)
			sql << "', '";
		first = false;
		sql << (*it).first;
	}
	sql << "') VALUES ('";
	first = true;
	for (auto it = fields.begin(); it != fields.end(); ++it)
	{
		if (!first)
			sql << "', '";
		first = false;
		sql << (*it).second;
	}
	sql << "');";
}

void MockGameDatabase::addGame(
	const std::string& 			fileid,
	const std::string& 			systemid,
	const std::string&			path,
	const std::string&			folder)
{
	std::stringstream sql;
	std::vector<std::pair<std::string, std::string> > fields;
	if (!fileid.empty()) fields.push_back(make_pair("fileid", fileid));
	if (!systemid.empty()) fields.push_back(make_pair("systemid", systemid));
	if (!path.empty()) fields.push_back(make_pair("path", path));
	if (!folder.empty()) fields.push_back(make_pair("folder", folder));

	buildSql("games", fields, sql);
	ASSERT_EQ(sqlite3_exec(mDB, sql.str().c_str(), NULL, NULL, NULL), SQLITE_OK);
}

void MockGameDatabase::addMetadata(
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
	const std::string&			players)
{
	std::stringstream sql;
	std::vector<std::pair<std::string, std::string> > fields;
	if (!fileid.empty()) fields.push_back(make_pair("fileid", fileid));
	if (!systemid.empty()) fields.push_back(make_pair("systemid", systemid));
	if (!name.empty()) fields.push_back(make_pair("name", name));
	if (!description.empty()) fields.push_back(make_pair("description", description));
	if (!image.empty()) fields.push_back(make_pair("image", image));
	if (!marquee.empty()) fields.push_back(make_pair("marquee", marquee));
	if (!snapshot.empty()) fields.push_back(make_pair("snapshot", snapshot));
	if (!thumbnail.empty()) fields.push_back(make_pair("thumbnail", thumbnail));
	if (!video.empty()) fields.push_back(make_pair("video", video));
	if (!releasedate.empty()) fields.push_back(make_pair("releasedate", releasedate));
	if (!developer.empty()) fields.push_back(make_pair("developer", developer));
	if (!publisher.empty()) fields.push_back(make_pair("publisher", publisher));
	if (!genre.empty()) fields.push_back(make_pair("genre", genre));
	if (!players.empty()) fields.push_back(make_pair("players", players));

	buildSql("metadata", fields, sql);
	ASSERT_EQ(sqlite3_exec(mDB, sql.str().c_str(), NULL, NULL, NULL), SQLITE_OK);
}

void MockGameDatabase::addStats(
	const std::string& 			fileid,
	const std::string& 			systemid,
	const std::string&			rating,
	const std::string&			playcount,
	const std::string&			groups)
{
	std::stringstream sql;
	std::vector<std::pair<std::string, std::string> > fields;
	if (!fileid.empty()) fields.push_back(make_pair("fileid", fileid));
	if (!systemid.empty()) fields.push_back(make_pair("systemid", systemid));
	if (!rating.empty()) fields.push_back(make_pair("rating", rating));
	if (!playcount.empty()) fields.push_back(make_pair("playcount", playcount));
	if (!groups.empty()) fields.push_back(make_pair("groups", groups));

	buildSql("stats", fields, sql);
	ASSERT_EQ(sqlite3_exec(mDB, sql.str().c_str(), NULL, NULL, NULL), SQLITE_OK);
}

void MockGameDatabase::addFolder(
	const std::string&			name,
	const std::string&			description,
	const std::string&			image,
	const std::string&			thumbnail,
	const std::string&			parent)
{
	std::stringstream sql;
	std::vector<std::pair<std::string, std::string> > fields;
	if (!name.empty()) fields.push_back(make_pair("name", name));
	if (!description.empty()) fields.push_back(make_pair("description", description));
	if (!image.empty()) fields.push_back(make_pair("image", image));
	if (!thumbnail.empty()) fields.push_back(make_pair("thumbnail", thumbnail));
	if (!parent.empty()) fields.push_back(make_pair("parent", parent));

	buildSql("folder", fields, sql);
	ASSERT_EQ(sqlite3_exec(mDB, sql.str().c_str(), NULL, NULL, NULL), SQLITE_OK);
}

void MockGameDatabase::addGroup(
	const std::string& 			name)
{
	std::stringstream sql;
	std::vector<std::pair<std::string, std::string> > fields;
	fields.push_back(make_pair("name", name));

	buildSql("group", fields, sql);
	ASSERT_EQ(sqlite3_exec(mDB, sql.str().c_str(), NULL, NULL, NULL), SQLITE_OK);
}
