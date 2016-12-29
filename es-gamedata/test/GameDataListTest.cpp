#include <gtest/gtest.h>
#include <string>
#include "GameDataList.h"
#include "GameDataItem.h"
#include "MockGameDatabase.h"

TEST(GameDataList, SingleGame) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");

	GameDataList gdl(db.mDB);

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "1942");
	ASSERT_EQ(item->systemID(), "arcade");
	ASSERT_EQ(item->path(), "~/Mame/1942.zip");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}


TEST(GameDataList, MultipleGames) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "", "3", "0");

	GameDataList gdl(db.mDB);

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "1942");
	ASSERT_EQ(item->systemID(), "arcade");
	ASSERT_EQ(item->path(), "~/Mame/1942.zip");

	// Make sure there is another game
	item = gdl.getNext();
	ASSERT_NE(item, nullptr);
	// Check the game data
	ASSERT_EQ(item->id(), "rtype");
	ASSERT_EQ(item->systemID(), "arcade");
	ASSERT_EQ(item->path(), "~/Mame/rtype.zip");

	// Make sure there is another game
	item = gdl.getNext();
	ASSERT_NE(item, nullptr);
	// Check the game data
	ASSERT_EQ(item->id(), "bublbobl");
	ASSERT_EQ(item->systemID(), "arcade");
	ASSERT_EQ(item->path(), "~/Mame/bublbobl.zip");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}

TEST(GameDataList, SingleGameMultipleSystems) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addGame("1943", "nes", "~/NES/1943.zip", "", "3", "0");

	GameDataList gdl(db.mDB);
	gdl.filterSystem("nes");

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "1943");
	ASSERT_EQ(item->systemID(), "nes");
	ASSERT_EQ(item->path(), "~/NES/1943.zip");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}

TEST(GameDataList, MultipleGamesMultipleSystems) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "mno");
	ASSERT_EQ(item->systemID(), "c64");
	ASSERT_EQ(item->path(), "~/c64/mno.zip");

	// Make sure there is another game
	item = gdl.getNext();
	ASSERT_NE(item, nullptr);
	// Check the game data
	ASSERT_EQ(item->id(), "pqr");
	ASSERT_EQ(item->systemID(), "c64");
	ASSERT_EQ(item->path(), "~/c64/pqr.zip");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}

/*
 * Query using a single tag with 'match any'
 */
TEST(GameDataList, SingleTagAny) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "rtype");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}

/*
 * Query using a single tag with 'match all'
 */
TEST(GameDataList, SingleTagAll) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "rtype");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}

/*
 * Query using a multiple tags with 'match any' where we only
 * search on a single tag
 */
TEST(GameDataList, MultipleTagAny) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "rtype");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}

/*
 * Query using a multiple tags with 'match any' where we search
 * on multiple tags
 */
TEST(GameDataList, MultipleTagAny2) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "rtype");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);

}

/*
 * Query using a multiple tags with 'match all' where we only
 * search on a single tag
 */
TEST(GameDataList, MultipleTagAll) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	// Make sure there are no games
	ASSERT_EQ(item, nullptr);
}

/*
 * Query using a multiple tags with 'match all' where we search
 * on multiple tags
 */
TEST(GameDataList, MultipleTagAll2) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "rtype");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);

}

/*
 * Query using a multiple tags with 'match any' where multiple games match
 */
TEST(GameDataList, MultipleTagAny3) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "rtype");

	// There should be two more
	item = gdl.getNext();
	ASSERT_NE(item, nullptr);
	ASSERT_EQ(item->id(), "bublbobl");

	item = gdl.getNext();
	ASSERT_NE(item, nullptr);
	ASSERT_EQ(item->id(), "mno");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}

/*
 * Query using a multiple tags with 'match all' where multiple games match
 */
TEST(GameDataList, MultipleTagAll3) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "rtype");

	// There should be two more
	item = gdl.getNext();
	ASSERT_NE(item, nullptr);
	ASSERT_EQ(item->id(), "bublbobl");

	item = gdl.getNext();
	ASSERT_NE(item, nullptr);
	ASSERT_EQ(item->id(), "mno");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}

/*
 * Query using a multiple tags with 'match any' where multiple games match with
 * a system filter
 */
TEST(GameDataList, MultipleTagAnySystem) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "rtype");

	// There should be one more
	item = gdl.getNext();
	ASSERT_NE(item, nullptr);
	ASSERT_EQ(item->id(), "bublbobl");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}

/*
 * Query using a multiple tags with 'match all' where multiple games match with
 * a system filter
 */
TEST(GameDataList, MultipleTagAllSystem) {
	MockGameDatabase db("/tmp/testdb.db");
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

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "rtype");

	// There should be one more
	item = gdl.getNext();
	ASSERT_NE(item, nullptr);
	ASSERT_EQ(item->id(), "bublbobl");

	// Make sure there are no more games
	ASSERT_EQ(gdl.getNext(), nullptr);
}

