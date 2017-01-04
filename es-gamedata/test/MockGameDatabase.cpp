/*
 * MockGameDatabase.cpp
 *
 *  Created on: 19 Dec 2016
 *      Author: rhopkins
 */

#include "MockGameDatabase.h"
#include <gtest/gtest.h>

MockGameDatabase::MockGameDatabase(std::string path) : mDB(nullptr), mPath(path)
{
	// If the path is not ":memory:" then remove any previous file
	if (mPath != ":memory:")
		remove(mPath.c_str());
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
		"fileid TEXT NOT NULL, " <<
		"systemid TEXT NOT NULL, " <<
		"tag TEXT, " <<
		"value TEXT)";
	ASSERT_EQ(sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL), SQLITE_OK);

	// Games
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS games (" <<
		"fileid TEXT NOT NULL, " <<
		"systemid TEXT NOT NULL, " <<
		"path TEXT, " <<
		"folder TEXT, " <<
		"rating INT DEFAULT 3, " <<
		"playcount INT DEFAULT 0, " <<
		"PRIMARY KEY (fileid, systemid))";
	ASSERT_EQ(sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL), SQLITE_OK);

	// Folders
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS folders (" <<
		"id INTEGER PRIMARY KEY, " <<
		"systemid TEXT NOT NULL, " <<
		"fullpath TEXT NOT NULL, " <<
		"name TEXT NOT NULL, " <<
		"description TEXT, " <<
		"image TEXT, " <<
		"thumbnail TEXT, " <<
		"parent TEXT" <<
		")";
	ASSERT_EQ(sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL), SQLITE_OK);

	// Tags
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS tags (" <<
		"fileid TEXT NOT NULL, " <<
		"systemid TEXT NOT NULL, " <<
		"tag TEXT NOT NULL" <<
		")";
	ASSERT_EQ(sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL), SQLITE_OK);
}

void MockGameDatabase::close()
{
	if (mDB)
		sqlite3_close(mDB);
	mDB = nullptr;
}

void MockGameDatabase::buildSql(std::string table, const std::vector<std::pair<std::string, std::string> >& fields, std::stringstream& sql)
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
	std::string 		fileid,
	std::string 		systemid,
	std::string			path,
	std::string			folder,
	std::string			rating,
	std::string			playcount)
{
	std::stringstream sql;
	std::vector<std::pair<std::string, std::string> > fields;
	if (!fileid.empty()) fields.push_back(make_pair("fileid", fileid));
	if (!systemid.empty()) fields.push_back(make_pair("systemid", systemid));
	if (!path.empty()) fields.push_back(make_pair("path", path));
	if (!folder.empty()) fields.push_back(make_pair("folder", folder));
	if (!rating.empty()) fields.push_back(make_pair("rating", rating));
	if (!playcount.empty()) fields.push_back(make_pair("playcount", playcount));

	buildSql("games", fields, sql);
	ASSERT_EQ(sqlite3_exec(mDB, sql.str().c_str(), NULL, NULL, NULL), SQLITE_OK);
}

void MockGameDatabase::addMetadata(
	std::string 			fileid,
	std::string 			systemid,
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
	std::string			players)
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

void MockGameDatabase::addFolder(
	std::string			id,
	std::string			systemid,
	std::string			fullpath,
	std::string			name,
	std::string			description,
	std::string			image,
	std::string			thumbnail,
	std::string			parent)
{
	std::stringstream sql;
	std::vector<std::pair<std::string, std::string> > fields;
	if (!id.empty()) fields.push_back(make_pair("id", id));
	if (!systemid.empty()) fields.push_back(make_pair("systemid", name));
	if (!fullpath.empty()) fields.push_back(make_pair("fullpath", name));
	if (!name.empty()) fields.push_back(make_pair("name", name));
	if (!description.empty()) fields.push_back(make_pair("description", description));
	if (!image.empty()) fields.push_back(make_pair("image", image));
	if (!thumbnail.empty()) fields.push_back(make_pair("thumbnail", thumbnail));
	if (!parent.empty()) fields.push_back(make_pair("parent", parent));

	buildSql("folder", fields, sql);
	ASSERT_EQ(sqlite3_exec(mDB, sql.str().c_str(), NULL, NULL, NULL), SQLITE_OK);
}

void MockGameDatabase::addTag(
		std::string 		fileid,
		std::string 		systemid,
		std::string 		tag)
{
	std::stringstream sql;
	std::vector<std::pair<std::string, std::string> > fields;
	fields.push_back(make_pair("fileid", fileid));
	fields.push_back(make_pair("systemid", systemid));
	fields.push_back(make_pair("tag", tag));

	buildSql("tags", fields, sql);
	ASSERT_EQ(sqlite3_exec(mDB, sql.str().c_str(), NULL, NULL, NULL), SQLITE_OK);
}
