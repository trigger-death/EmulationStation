#include <gtest/gtest.h>
#include <string>
#include "GameDataList.h"
#include "GameDataItem.h"
#include "MockGameDatabase.h"

TEST(GameDataList, SingleGame) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");

	std::vector<std::string> 	tags;
	GameDataList gdl(db.mDB, "arcade", tags);

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

	std::vector<std::string> 	tags;
	GameDataList gdl(db.mDB, "arcade", tags);

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
