#include <gtest/gtest.h>
#include <string>
#include "GameDataFolder.h"
#include "GameDataItem.h"
#include "MockGameDatabase.h"

TEST(GameDataFolderTest, GetItems) {
	MockGameDatabase db;
	db.create();
	db.addGame("1942", "arcade", "1942.zip", "0", "3", "0");
	db.addGame("rtype", "arcade", "rtype.zip", "0", "3", "0");

	GameDataFolder folder;
	folder.addGame(new GameDataGame(db.mDB, "1942", "arcade", "1942.zip", 0, 0));
	folder.addGame(new GameDataGame(db.mDB, "rtype", "arcade", "rtype.zip", 0, 0));

	ASSERT_EQ(folder.items().size(), 2);
}

