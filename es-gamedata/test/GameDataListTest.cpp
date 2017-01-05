#include <gtest/gtest.h>
#include <string>
#include "GameDataList.h"
#include "GameDataItem.h"
#include "GameDataFolder.h"
#include "MockGameDatabase.h"

TEST(GameDataList, SingleGame) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");

	GameDataList gdl(db.mDB);

	// Check the game data
	ASSERT_EQ(gdl.folder()->items().size(), 1);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "1942");
	ASSERT_EQ(game->systemID(), "arcade");
	ASSERT_EQ(game->path(), "~/Mame/1942.zip");

}

TEST(GameDataList, MultipleGames) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");

	GameDataList gdl(db.mDB);

	// Check the game data
	ASSERT_EQ(gdl.folder()->items().size(), 3);

	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "1942");
	ASSERT_EQ(game->systemID(), "arcade");
	ASSERT_EQ(game->path(), "~/Mame/1942.zip");

	game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[1]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");
	ASSERT_EQ(game->systemID(), "arcade");
	ASSERT_EQ(game->path(), "~/Mame/rtype.zip");

	game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[2]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "bublbobl");
	ASSERT_EQ(game->systemID(), "arcade");
	ASSERT_EQ(game->path(), "~/Mame/bublbobl.zip");
}

TEST(GameDataList, SingleGameMultipleSystems) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("1943", "nes", "~/NES/1943.zip", "", "3", "0");

	GameDataList gdl(db.mDB);
	gdl.filterSystem("nes");

	// Check the game data
	ASSERT_EQ(gdl.folder()->items().size(), 1);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "1943");
	ASSERT_EQ(game->systemID(), "nes");
	ASSERT_EQ(game->path(), "~/NES/1943.zip");
}

TEST(GameDataList, MultipleGamesMultipleSystems) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	GameDataList gdl(db.mDB);
	gdl.filterSystem("c64");

	ASSERT_EQ(gdl.folder()->items().size(), 2);

	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "mno");
	ASSERT_EQ(game->systemID(), "c64");
	ASSERT_EQ(game->path(), "~/c64/mno.zip");

	game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[1]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "pqr");
	ASSERT_EQ(game->systemID(), "c64");
	ASSERT_EQ(game->path(), "~/c64/pqr.zip");
}

/*
 * Query using a single tag with 'match any'
 */
TEST(GameDataList, SingleTagAny) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	db.addTag("1942", "arcade", "favorite");
	db.addTag("rtype", "arcade", "scroller");

	// Setup the filter tags
	std::set<std::string> tags;
	tags.insert("scroller");

	// Build the list and filter it
	GameDataList gdl(db.mDB);
	gdl.filterTags(tags, false);

	ASSERT_EQ(gdl.folder()->items().size(), 1);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");
}

/*
 * Query using a single tag with 'match all'
 */
TEST(GameDataList, SingleTagAll) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	db.addTag("1942", "arcade", "favorite");
	db.addTag("rtype", "arcade", "scroller");

	// Setup the filter tags
	std::set<std::string> tags;
	tags.insert("scroller");

	// Build the list and filter it
	GameDataList gdl(db.mDB);
	gdl.filterTags(tags, true);

	ASSERT_EQ(gdl.folder()->items().size(), 1);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");
}

/*
 * Query using a multiple tags with 'match any' where we only
 * search on a single tag
 */
TEST(GameDataList, MultipleTagAny) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	db.addTag("1942", "arcade", "favorite");
	db.addTag("rtype", "arcade", "scroller");
	db.addTag("rtype", "arcade", "another");

	// Setup the filter tags
	std::set<std::string> tags;
	tags.insert("scroller");

	// Build the list and filter it
	GameDataList gdl(db.mDB);
	gdl.filterTags(tags, false);

	ASSERT_EQ(gdl.folder()->items().size(), 1);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");
}

/*
 * Query using a multiple tags with 'match any' where we search
 * on multiple tags
 */
TEST(GameDataList, MultipleTagAny2) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	db.addTag("1942", "arcade", "favorite");
	db.addTag("rtype", "arcade", "scroller");
	db.addTag("rtype", "arcade", "another");

	// Setup the filter tags
	std::set<std::string> tags;
	tags.insert("scroller");
	tags.insert("another");

	// Build the list and filter it
	GameDataList gdl(db.mDB);
	gdl.filterTags(tags, false);

	ASSERT_EQ(gdl.folder()->items().size(), 1);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");
}

/*
 * Query using a multiple tags with 'match all' where we only
 * search on a single tag
 */
TEST(GameDataList, MultipleTagAll) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	db.addTag("1942", "arcade", "favorite");
	db.addTag("rtype", "arcade", "scroller");
	db.addTag("rtype", "arcade", "another");

	// Setup the filter tags
	std::set<std::string> tags;
	tags.insert("scroller");
	tags.insert("no_exist");		// Doesn't exist

	// Build the list and filter it
	GameDataList gdl(db.mDB);
	gdl.filterTags(tags, true);

	ASSERT_EQ(gdl.folder()->items().size(), 0);
}

/*
 * Query using a multiple tags with 'match all' where we search
 * on multiple tags
 */
TEST(GameDataList, MultipleTagAll2) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	db.addTag("1942", "arcade", "favorite");
	db.addTag("rtype", "arcade", "scroller");
	db.addTag("rtype", "arcade", "another");

	// Setup the filter tags
	std::set<std::string> tags;
	tags.insert("scroller");
	tags.insert("another");

	// Build the list and filter it
	GameDataList gdl(db.mDB);
	gdl.filterTags(tags, true);

	ASSERT_EQ(gdl.folder()->items().size(), 1);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");
}

/*
 * Query using a multiple tags with 'match any' where multiple games match
 */
TEST(GameDataList, MultipleTagAny3) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	db.addTag("1942", "arcade", "favorite");
	db.addTag("rtype", "arcade", "scroller");
	db.addTag("rtype", "arcade", "another");
	db.addTag("rtype", "arcade", "notused");
	db.addTag("bublbobl", "arcade", "scroller");
	db.addTag("bublbobl", "arcade", "another");
	db.addTag("bublbobl", "arcade", "hello");
	db.addTag("mno", "c64", "scroller");
	db.addTag("mno", "c64", "another");

	// Setup the filter tags
	std::set<std::string> tags;
	tags.insert("scroller");

	// Build the list and filter it
	GameDataList gdl(db.mDB);
	gdl.filterTags(tags, false);

	ASSERT_EQ(gdl.folder()->items().size(), 3);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");

	game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[1]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "bublbobl");

	game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[2]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "mno");
}

/*
 * Query using a multiple tags with 'match all' where multiple games match
 */
TEST(GameDataList, MultipleTagAll3) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	db.addTag("1942", "arcade", "favorite");
	db.addTag("rtype", "arcade", "scroller");
	db.addTag("rtype", "arcade", "another");
	db.addTag("rtype", "arcade", "notused");
	db.addTag("bublbobl", "arcade", "scroller");
	db.addTag("bublbobl", "arcade", "another");
	db.addTag("bublbobl", "arcade", "hello");
	db.addTag("mno", "c64", "scroller");
	db.addTag("mno", "c64", "another");

	// Setup the filter tags
	std::set<std::string> tags;
	tags.insert("scroller");
	tags.insert("another");

	// Build the list and filter it
	GameDataList gdl(db.mDB);
	gdl.filterTags(tags, true);

	ASSERT_EQ(gdl.folder()->items().size(), 3);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");

	game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[1]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "bublbobl");

	game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[2]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "mno");
}

/*
 * Query using a multiple tags with 'match any' where multiple games match with
 * a system filter
 */
TEST(GameDataList, MultipleTagAnySystem) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	db.addTag("1942", "arcade", "favorite");
	db.addTag("rtype", "arcade", "scroller");
	db.addTag("rtype", "arcade", "another");
	db.addTag("rtype", "arcade", "notused");
	db.addTag("bublbobl", "arcade", "scroller");
	db.addTag("bublbobl", "arcade", "another");
	db.addTag("bublbobl", "arcade", "hello");
	db.addTag("mno", "c64", "scroller");
	db.addTag("mno", "c64", "another");

	// Setup the filter tags
	std::set<std::string> tags;
	tags.insert("scroller");

	// Build the list and filter it
	GameDataList gdl(db.mDB);
	gdl.filterTags(tags, false);
	gdl.filterSystem("arcade");

	ASSERT_EQ(gdl.folder()->items().size(), 2);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");

	game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[1]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "bublbobl");
}

/*
 * Query using a multiple tags with 'match all' where multiple games match with
 * a system filter
 */
TEST(GameDataList, MultipleTagAllSystem) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "", "3", "0");

	db.addTag("1942", "arcade", "favorite");
	db.addTag("rtype", "arcade", "scroller");
	db.addTag("rtype", "arcade", "another");
	db.addTag("rtype", "arcade", "notused");
	db.addTag("bublbobl", "arcade", "scroller");
	db.addTag("bublbobl", "arcade", "another");
	db.addTag("bublbobl", "arcade", "hello");
	db.addTag("mno", "c64", "scroller");
	db.addTag("mno", "c64", "another");

	// Setup the filter tags
	std::set<std::string> tags;
	tags.insert("scroller");
	tags.insert("another");

	// Build the list and filter it
	GameDataList gdl(db.mDB);
	gdl.filterTags(tags, true);
	gdl.filterSystem("arcade");

	ASSERT_EQ(gdl.folder()->items().size(), 2);
	GameDataGame* game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");

	game = dynamic_cast<GameDataGame*>(gdl.folder()->items()[1]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "bublbobl");
}

TEST(GameDataList, FolderHierarchy) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "Mame/0-9/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "Mame/R/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "Mame/B/bublbobl.zip", "", "3", "0");

	GameDataList gdl(db.mDB);
	gdl.filterFlat(false);

	// Check the game data
	ASSERT_EQ(gdl.folder()->items().size(), 1);

	// Should be a folder
	GameDataFolder* folder = dynamic_cast<GameDataFolder*>(gdl.folder()->items()[0]);
	ASSERT_NE(folder, nullptr);

	// Folder should contain 3 subfolders
	ASSERT_EQ(folder->items().size(), 3);

	// Each folder should contain 1 game
	GameDataFolder* subfolder = dynamic_cast<GameDataFolder*>(folder->items()[0]);
	ASSERT_NE(subfolder, nullptr);
	ASSERT_EQ(subfolder->name(), "0-9");
	ASSERT_EQ(subfolder->items().size(), 1);
	GameDataGame* game = dynamic_cast<GameDataGame*>(subfolder->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "1942");

	subfolder = dynamic_cast<GameDataFolder*>(folder->items()[1]);
	ASSERT_NE(subfolder, nullptr);
	ASSERT_EQ(subfolder->name(), "R");
	ASSERT_EQ(subfolder->items().size(), 1);
	game = dynamic_cast<GameDataGame*>(subfolder->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "rtype");

	subfolder = dynamic_cast<GameDataFolder*>(folder->items()[2]);
	ASSERT_NE(subfolder, nullptr);
	ASSERT_EQ(subfolder->name(), "B");
	ASSERT_EQ(subfolder->items().size(), 1);
	game = dynamic_cast<GameDataGame*>(subfolder->items()[0]);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "bublbobl");
}
