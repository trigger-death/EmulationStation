#include <gtest/gtest.h>
#include <string>
#include "GameDataFolder.h"
#include "GameDataItem.h"
#include "MockGameDatabase.h"

TEST(GameDataFolderTest, GetItems) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "0", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "0", "3", "0");

	GameDataFolder folder(db.mDB, "SELECT * FROM games", "");
	GameDataItem* item = folder.getFirstItem();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "1942");
	ASSERT_EQ(item->systemID(), "arcade");
	ASSERT_EQ(item->path(), "~/Mame/1942.zip");

	// Make sure there is another game
	item = folder.getNextItem();
	ASSERT_NE(item, nullptr);
	// Check the game data
	ASSERT_EQ(item->id(), "rtype");
	ASSERT_EQ(item->systemID(), "arcade");
	ASSERT_EQ(item->path(), "~/Mame/rtype.zip");

	// Make sure there are no more games
	ASSERT_EQ(folder.getNextItem(), nullptr);
}

/*
 * Test that we can filter results from a query based on folder ID
 */
TEST(GameDataFolderTest, FolderFilter) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "0", "3", "0");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "0", "3", "0");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "1", "3", "0");
	db.addGame("abc", "nes", "~/nes/abc.zip", "2", "3", "0");
	db.addGame("def", "snes", "~/snes/def.zip", "4", "3", "0");
	db.addGame("ghi", "snes", "~/snes/ghi.zip", "5", "3", "0");
	db.addGame("jkl", "spectrum", "~/jkl/1942.zip", "6", "3", "0");
	db.addGame("mno", "c64", "~/c64/mno.zip", "7", "3", "0");
	db.addGame("pqr", "c64", "~/c64/pqr.zip", "7", "3", "0");

	GameDataFolder folder(db.mDB, "SELECT * FROM games", "1");
	GameDataItem* item = folder.getFirstItem();
	ASSERT_NE(item, nullptr);

	// Check the game data
	ASSERT_EQ(item->id(), "bublbobl");

	// Make sure there are no more games
	ASSERT_EQ(folder.getNextItem(), nullptr);
}

/*
 * Test that we can create a new relative folder from a root folder
 */
TEST(GameDataFolderTest, CreateRelativeFolder) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();

	mkdir("/tmp/es-games", S_IRWXU);
	mkdir("/tmp/es-games/A", S_IRWXU);
	FILE* fp = fopen("/tmp/es-games/A/abc.zip", "wb");
	fclose(fp);

	std::string folder = GameDataFolder::getGameFolder(db.mDB, "arcade", "/tmp/es-games", "/tmp/es-games/A/abc.zip", "");
	ASSERT_NE(folder, "");
}

/*
 * Test that we can create a new relative folder from a parent folder
 */
TEST(GameDataFolderTest, CreateRelativeFolderParent) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();

	mkdir("/tmp/es-games", S_IRWXU);
	mkdir("/tmp/es-games/A", S_IRWXU);
	FILE* fp = fopen("/tmp/es-games/A/abc.zip", "wb");
	fclose(fp);

	std::string parent = GameDataFolder::getGameFolder(db.mDB, "arcade", "/tmp/es-games", "/tmp/es-games", "");
	ASSERT_NE(parent, "");
	std::string folder = GameDataFolder::getGameFolder(db.mDB, "arcade", "/tmp/es-games", "/tmp/es-games/A/abc.zip", parent);
	ASSERT_NE(folder, "");
}
