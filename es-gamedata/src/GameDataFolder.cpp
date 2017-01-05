/*
 * GameDataFolder.cpp
 *
 *  Created on: 30 Dec 2016
 *      Author: rhopkins
 */

#include "GameDataFolder.h"
#include "GameDataItem.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

GameDataFolder::GameDataFolder(std::string name) : GameDataItem(), mName(name)
{
}

GameDataFolder::~GameDataFolder()
{
	for (auto item : mItems)
		delete item;
}

GameDataFolder* GameDataFolder::parent()
{
	return nullptr;
}

const std::vector<GameDataItem*> GameDataFolder::items()
{
	return mItems;
}

void GameDataFolder::addGame(GameDataGame* game, boost::filesystem::path path)
{
	fs::path 	subpath;
	fs::path 	subdir;
	// The game path contains the relative path from the system root up until the game
	// file. We want to split the path components into sub folders and recursively
	// call this function
	if (path.empty())
		path = game->path();
	bool first = true;
	for (auto part : path)
	{
		// Capture the first element as the folder. Put the remainder into the filename
		// to recursively call this function
		if (first)
		{
			first = false;
			subdir = part;
		}
		else
			subpath /= part;
	}
	// If the subpath is empty then there was only a single component so add the game
	// to this folder
	if (subpath.empty())
	{
		mItems.push_back(game);
	}
	else
	{
		// Need to recurse. Find the correct child folder
		auto folder_it = mFolders.find(subdir.generic_string());
		if (folder_it == mFolders.end())
		{
			GameDataFolder* folder = new GameDataFolder(subdir.generic_string());
			mFolders[subdir.generic_string()] = folder;
			mItems.push_back(folder);
			folder->addGame(game, subpath);
		}
		else
			(*folder_it).second->addGame(game, subpath);
	}
}
