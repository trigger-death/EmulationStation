/*
 * GameData.cpp
 *
 *  Created on: 18 Dec 2016
 *      Author: rhopkins
 */

#include <string>
#include <stdio.h>
#include "GameData.h"
#include "GameDataException.h"
#include "pugixml/pugixml.hpp"
#include "Util.h"
#include "MetaData.h"
#include "PlatformId.h"

namespace fs = boost::filesystem;

/*
 * Singleton instance
 */
GameData	GameData::mGameData;

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
		sqlite3_exec(mDB, "BEGIN TRANSACTION", NULL, NULL, NULL);
		for (pugi::xml_node fileNode = root.child(tag); fileNode; fileNode = fileNode.next_sibling(tag))
		{
			// Get the path
			fs::path path = resolvePath(fileNode.child("path").text().get(), relativeTo, false);
			// Get the metadata including a default name
			std::string defaultName = path.stem().generic_string();

			addGame(system, path.generic_string(), defaultName);

			MetaDataList mdl = MetaDataList::createFromXML(GAME_METADATA, fileNode, relativeTo);
			//make sure name gets set if one didn't exist
			if (mdl.get("name").empty())
			{
				if (system.hasPlatformId(PlatformIds::ARCADE) || system.hasPlatformId(PlatformIds::NEOGEO))
					defaultName = PlatformIds::getCleanMameName(defaultName.c_str());
				mdl.set("name", defaultName);
			}

			// Populate the metadata in the database
			for (auto it = mdl.getMetadata().begin(); it != mdl.getMetadata().end(); ++it)
				addMetadata(system, defaultName, it->first, it->second);
		}
		sqlite3_exec(mDB, "END TRANSACTION", NULL, NULL, NULL);
	}
}

GameDataList* GameData::createGameList()
{
	return new GameDataList(mDB);
}

void GameData::createTables()
{
	std::stringstream ss;

	// Metadata
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS metadata (" <<
		"fileid TEXT NOT NULL, " <<
		"systemid TEXT NOT NULL, " <<
		"tag TEXT, " <<
		"value TEXT)";
	if (sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL))
		LOG(LogError) << "Could not create metadata table: " << sqlite3_errmsg(mDB) << std::endl;

	// Games
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS games (" <<
		"fileid TEXT NOT NULL, " <<
		"systemid TEXT NOT NULL, " <<
		"path TEXT, " <<
		"rating INT DEFAULT 3, " <<
		"playcount INT DEFAULT 0, " <<
		"PRIMARY KEY (fileid, systemid))";
	if (sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL))
		LOG(LogError) << "Could not create games table: " << sqlite3_errmsg(mDB) << std::endl;

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
	if (sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL))
		LOG(LogError) << "Could not create folders table: " << sqlite3_errmsg(mDB) << std::endl;

	// Tags
	ss.str("");
	ss << "CREATE TABLE IF NOT EXISTS tags (" <<
		"fileid TEXT NOT NULL, " <<
		"systemid TEXT NOT NULL, " <<
		"tag TEXT NOT NULL" <<
		")";
	if (sqlite3_exec(mDB, ss.str().c_str(), NULL, NULL, NULL))
		LOG(LogError) << "Could not create groups table: " << sqlite3_errmsg(mDB) << std::endl;

}

void GameData::populateFolder(const GameDataSystem& system, std::string rootPath, std::vector<std::string> extensions)
{
	sqlite3_exec(mDB, "BEGIN TRANSACTION", NULL, NULL, NULL);
	populateFolderInternal(system, rootPath, extensions);
	sqlite3_exec(mDB, "END TRANSACTION", NULL, NULL, NULL);
}

void GameData::populateFolderInternal(const GameDataSystem& system, std::string rootPath, std::vector<std::string> extensions)
{
	const fs::path& folderPath = rootPath;
	if( !fs::is_directory(folderPath))
	{
		LOG(LogWarning) << "Error - folder with path \"" << folderPath << "\" is not a directory!";
		return;
	}

	const std::string folderStr = folderPath.generic_string();

	// Make sure that this isn't a symlink to a thing we already have
	if (fs::is_symlink(folderPath))
	{
		// If this symlink resolves to somewhere that's at the beginning of our path, it's gonna recurse
		if (folderStr.find(fs::canonical(folderPath).generic_string()) == 0)
		{
			LOG(LogWarning) << "Skipping infinitely recursive symlink \"" << folderPath << "\"";
			return;
		}
	}

	fs::path filePath;
	std::string extension;
	for (fs::directory_iterator end, dir(folderPath); dir != end; ++dir)
	{
		filePath = (*dir).path();

		if (filePath.stem().empty())
			continue;

		// this is a little complicated because we allow a list of extensions to be defined (delimited with a space)
		// we first get the extension of the file itself:
		extension = filePath.extension().string();

		// fyi, folders *can* also match the extension and be added as games - this is mostly just to support higan
		// see issue #75: https://github.com/Aloshi/EmulationStation/issues/75
		if (std::find(extensions.begin(), extensions.end(), extension) != extensions.end())
		{
			// It is a game
			std::string stem = filePath.stem().generic_string();
			addGame(system, filePath.generic_string(), stem);

			std::string name = stem;
			if (system.hasPlatformId(PlatformIds::ARCADE) || system.hasPlatformId(PlatformIds::NEOGEO))
				name = PlatformIds::getCleanMameName(name.c_str());

			// Populate the metadata in the database
			addMetadata(system, stem, "name", name);

		}
		else if (fs::is_directory(filePath))
		{
			populateFolder(system, filePath.generic_string(), extensions);
		}
	}
}

void GameData::addGame(const GameDataSystem& system, std::string path, std::string name)
{
	// Find the path relative to the system root directory
	bool contains;
	// Find the relative path
	fs::path rel = removeCommonPath(path, system.getRootPath(), contains);
	if (!contains)
	{
		LOG(LogError) << "Game path \"" << path << "\" is outside system path \"" << system.getRootPath() << "\"";
		return;
	}
	// Find the filename and use it as the game ID
	if (name.empty())
	{
		name = rel.stem().generic_string();
	}

	// See if it already exists
	if (!gameExists(system.id(), name))
	{
		// Attempt to insert it. The system ID and game ID form a unique ID so the insert will
		// fail if it already exists
		std::stringstream 	query;
		char* err;
		query << "INSERT INTO games ('fileid', 'systemid', 'path') VALUES ('" <<
				name << "', '" << system.id() << "', '" << rel.generic_string() << "')";
		int sqlret = sqlite3_exec(mDB, query.str().c_str(), NULL, NULL, &err);
		if ((sqlret != SQLITE_OK) && (sqlret != SQLITE_CONSTRAINT))
		{
			std::stringstream error;
			error << "Failed to insert game: " << path << ", Error: " << err;
			throw GameDataException(error.str());
		}
	}
}

bool GameData::gameExists(std::string systemId, std::string gameId)
{
	bool 				exists = false;
	std::stringstream 	query;
	sqlite3_stmt*		result;
	query << "SELECT COUNT(*) FROM games WHERE systemid='" << systemId << "' AND fileid='" << gameId << "'";
	int sqlret = sqlite3_prepare_v2(mDB, query.str().c_str(), query.str().size(), &result, NULL);
	if (sqlret != SQLITE_OK)
	{
		std::stringstream error;
		error << "Failed to query game: " << systemId << ", " << gameId << "Error: " << sqlret;
		throw GameDataException(error.str());
	}
	if (sqlite3_column_int(result, 0) > 0)
		exists = true;

	sqlite3_finalize(result);

	return exists;
}

void GameData::addMetadata(const GameDataSystem& system, std::string id, std::string tag, std::string value)
{
	std::stringstream	query;
	char*				err;

	size_t pos;
	for (pos = 0; pos < value.size(); ++pos)
	{
		if (value[pos] == '\'')
			value[pos] = '\"';
	}

	query << "INSERT INTO metadata ('systemid', 'fileid', 'tag', 'value') VALUES ('" << system.id() << "', '" <<
		     id << "', '" << tag << "', '" << value << "')";

	int sqlret = sqlite3_exec(mDB, query.str().c_str(), NULL, NULL, &err);
	if ((sqlret != SQLITE_OK) && (sqlret != SQLITE_CONSTRAINT))
	{
		std::stringstream error;
		error << "Failed to insert metadata for: " << system.id() << ", " << id << ", Error: " << err;
		std::cerr << error.str() << std::endl;
		std::cerr << query.str() << std::endl;
		exit(1);
		throw GameDataException(error.str());
	}
}
