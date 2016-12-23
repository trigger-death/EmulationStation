/*
 * GameData.cpp
 *
 *  Created on: 18 Dec 2016
 *      Author: rhopkins
 */

#include <string>
#include <stdio.h>
#include "GameData.h"
#include "pugixml/pugixml.hpp"
#include "Util.h"
#include "MetaData.h"

namespace fs = boost::filesystem;

GameData::GameData() : mIsOpen(false), mDB(NULL)
{
}

GameData::~GameData()
{
	if (mIsOpen)
		sqlite3_close(mDB);
	for (auto it = mGames.begin(); it != mGames.end(); ++it)
		delete it->second;
}

bool GameData::openDatabase(const boost::filesystem::path& path)
{
	// Make sure we haven't already opened a database
	if (mIsOpen)
	{
		LOG(LogWarning) << "Cannot open database - another database is already open" << std::endl;
		return false;
	}
	int result = sqlite3_open_v2(path.c_str(), &mDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (result != SQLITE_OK)
	{
		LOG(LogError) << "Failed to open/create database: " << path << ". Error " << result << std::endl;
		return false;
	}
	LOG(LogInfo) << "Database opened/created ok" << std::endl;

	// Attempt to create any missing tables
	createTables();

	return true;
}

void GameData::parseGameList(const GameDataSystem& system, const boost::filesystem::path& path)
{
	if (!boost::filesystem::exists(path))
		return;

	LOG(LogInfo) << "Parsing XML file \"" << path << "\"...";

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(path.c_str());

	if (!result)
	{
		LOG(LogError) << "Error parsing XML file \"" << path << "\"!\n	" << result.description();
		return;
	}

	pugi::xml_node root = doc.child("gameList");
	if(!root)
	{
		LOG(LogError) << "Could not find <gameList> node in gamelist \"" << path << "\"!";
		return;
	}

	fs::path relativeTo = system.getRootPath();

	const char* tagList[2] = { "game", "folder" };
	for (int i = 0; i < 2; i++)
	{
		const char* tag = tagList[i];
		for (pugi::xml_node fileNode = root.child(tag); fileNode; fileNode = fileNode.next_sibling(tag))
		{
			// Get the path
			fs::path path = resolvePath(fileNode.child("path").text().get(), relativeTo, false);
			// Get the metadata including a default name
			std::string defaultName = path.stem().generic_string();
			if (system.hasPlatformId(PlatformIds::ARCADE) || system.hasPlatformId(PlatformIds::NEOGEO))
				defaultName = PlatformIds::getCleanMameName(defaultName.c_str());

			MetaDataList mdl = MetaDataList::createFromXML(GAME_METADATA, fileNode, relativeTo);
			//make sure name gets set if one didn't exist
			if (mdl.get("name").empty())
				mdl.set("name", defaultName);
		}
	}
}

GameDataList& GameData::getGameList(const GameDataSystem& system, bool flat)
{
	auto it = mGames.find(system.id());
	if (it == mGames.end())
	{
		mGames[system.id()] = new GameDataList(mDB, system, flat);
	}
	return *mGames[system.id()];
}

void GameData::createTables()
{
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
	if (sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL))
		LOG(LogError) << "Could not create metadata table: " << sqlite3_errmsg(mDB) << std::endl;

	// Stats
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS stats (" <<
		"fileid VARCHAR(255) NOT NULL, " <<
		"systemid VARCHAR(255) NOT NULL, " <<
		"rating INT DEFAULT 3, " <<
		"playcount INT DEFAULT 0, " <<
		"groups TEXT, " <<
		"PRIMARY KEY (fileid, systemid))";
	if (sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL))
		LOG(LogError) << "Could not create stats table: " << sqlite3_errmsg(mDB) << std::endl;

	// Games
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS games (" <<
		"fileid VARCHAR(255) NOT NULL, " <<
		"systemid VARCHAR(255) NOT NULL, " <<
		"path TEXT, " <<
		"folder TEXT, " <<
		"PRIMARY KEY (fileid, systemid))";
	if (sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL))
		LOG(LogError) << "Could not create games table: " << sqlite3_errmsg(mDB) << std::endl;

	// Folders
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS folders (" <<
		"name TEXT NOT NULL, " <<
		"description TEXT, " <<
		"image TEXT, " <<
		"thumbnail TEXT, " <<
		"parent TEXT, " <<
		"PRIMARY KEY (name))";
	if (sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL))
		LOG(LogError) << "Could not create folders table: " << sqlite3_errmsg(mDB) << std::endl;

	// Groups
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS groups (" <<
		"name TEXT NOT NULL, " <<
		"PRIMARY KEY (name))";
	if (sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL))
		LOG(LogError) << "Could not create groups table: " << sqlite3_errmsg(mDB) << std::endl;

}
