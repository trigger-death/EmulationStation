#include <gtest/gtest.h>
#include <string>
#include "GameData.h"
#include "GameDataItemFolder.h"
#include "GameDataItemGame.h"
#include "GameDataSystem.h"
#include "MockGameDatabase.h"

TEST(GameData, OpenDatabase) {
	GameData	gd;
	ASSERT_TRUE(gd.openDatabase("/tmp/testdb.db"));
	remove("/tmp/testdb.db");
}

TEST(GameData, SingleGame) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "0-9");
	db.close();

	GameData 		gd;
	GameDataSystem	system("arcade", "~/roms/Mame");

	// Open the test database
	ASSERT_TRUE(gd.openDatabase("/tmp/testdb.db"));
	GameDataList& gdl = gd.getGameList(system, true);
	GameDataItemFolder* folder = gdl.root();
	ASSERT_NE(folder, nullptr);

	GameDataItem* item = folder->firstChild();
	ASSERT_NE(item, nullptr);

	// Check that the item that was returned is a game
	ASSERT_EQ(item->getType(), GameDataItem::GAME);
	GameDataItemGame* game = dynamic_cast<GameDataItemGame*>(item);
	ASSERT_NE(game, nullptr);
	// Check the game data
	ASSERT_EQ(game->id(), "1942");
	ASSERT_EQ(game->systemID(), "arcade");
	ASSERT_EQ(game->path(), "~/Mame/1942.zip");

	// Make sure there are no more games
	ASSERT_EQ(folder->getNextChild(), nullptr);
}


TEST(GameData, MultipleGames) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "0-9");
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "R");
	db.addGame("bublbobl", "arcade", "~/Mame/bublbobl.zip", "B");
	db.close();

	GameData 		gd;
	GameDataSystem	system("arcade", "~/roms/Mame");

	// Open the test database
	ASSERT_TRUE(gd.openDatabase("/tmp/testdb.db"));
	GameDataList& gdl = gd.getGameList(system, true);
	GameDataItemFolder* folder = gdl.root();
	ASSERT_NE(folder, nullptr);

	GameDataItem* item = folder->firstChild();
	ASSERT_NE(item, nullptr);

	// Check that the item that was returned is a game
	ASSERT_EQ(item->getType(), GameDataItem::GAME);
	GameDataItemGame* game = dynamic_cast<GameDataItemGame*>(item);
	ASSERT_NE(game, nullptr);
	// Check the game data
	ASSERT_EQ(game->id(), "1942");
	ASSERT_EQ(game->systemID(), "arcade");
	ASSERT_EQ(game->path(), "~/Mame/1942.zip");

	// Make sure there is another game
	item = folder->getNextChild();
	ASSERT_NE(item, nullptr);
	game = dynamic_cast<GameDataItemGame*>(item);
	ASSERT_NE(game, nullptr);
	// Check the game data
	ASSERT_EQ(game->id(), "rtype");
	ASSERT_EQ(game->systemID(), "arcade");
	ASSERT_EQ(game->path(), "~/Mame/rtype.zip");

	// Make sure there is another game
	item = folder->getNextChild();
	ASSERT_NE(item, nullptr);
	game = dynamic_cast<GameDataItemGame*>(item);
	ASSERT_NE(game, nullptr);
	// Check the game data
	ASSERT_EQ(game->id(), "bublbobl");
	ASSERT_EQ(game->systemID(), "arcade");
	ASSERT_EQ(game->path(), "~/Mame/bublbobl.zip");

	// Make sure there are no more games
	ASSERT_EQ(folder->getNextChild(), nullptr);
}

TEST(GameData, Metadata) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "0-9");
	db.addMetadata("1942", "arcade", "1942", "A shooting game", "~/Images/1942.png", "~/Marquees/1942.png",
				"~/Snapshots/1942.png", "~/Thumbnails/1942.png", "~/Videos/1942.wma", "1985", "Konami",
				"Konami", "Vertical Shooter", "1");
	db.addStats("1942", "arcade", "2", "0", "");
	db.close();

}
