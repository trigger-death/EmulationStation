#include <gtest/gtest.h>
#include <string>
#include "GameData.h"
#include "GameDataItemFolder.h"
#include "GameDataItemGame.h"
#include "GameDataSystem.h"
#include "MockGameDatabase.h"

TEST(Example, ex1) {
	GameData	gd;
	ASSERT_TRUE(gd.openDatabase("/tmp/testdb.db"));
	remove("/tmp/testdb.db");
}

TEST(Example, ex2) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "0-9");
	db.addMetadata("1942", "arcade", "1942", "A shooting game", "~/Images/1942.png", "~/Marquees/1942.png",
				"~/Snapshots/1942.png", "~/Thumbnails/1942.png", "~/Videos/1942.wma", "1985", "Konami",
				"Konami", "Vertical Shooter", "1");
	db.addStats("1942", "arcade", "2", "0", "");
	db.close();

	GameData 		gd;
	GameDataSystem	system("arcade", "~/roms/Mame");

	ASSERT_TRUE(gd.openDatabase("/tmp/testdb.db"));
	GameDataList& gdl = gd.getGameList(system, true);
	GameDataItemFolder* folder = gdl.root();
	ASSERT_NE(folder, nullptr);

	GameDataItem* item = folder->firstChild();
	ASSERT_NE(item, nullptr);

	ASSERT_EQ(item->getType(), GameDataItem::GAME);
	GameDataItemGame* game = dynamic_cast<GameDataItemGame*>(item);
	ASSERT_NE(game, nullptr);
	ASSERT_EQ(game->id(), "1942");
	ASSERT_EQ(game->systemID(), "arcade");
	ASSERT_EQ(game->path(), "~/Mame/1942.zip");
}
