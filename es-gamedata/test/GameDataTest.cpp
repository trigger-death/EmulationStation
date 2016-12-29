#include <gtest/gtest.h>
#include <string>
#include "GameData.h"
#include "GameDataItem.h"
#include "MockGameDatabase.h"

TEST(GameData, OpenDatabase) {
	GameData	gd;
	ASSERT_TRUE(gd.openDatabase("/tmp/testdb.db"));
	remove("/tmp/testdb.db");
}

TEST(GameData, GetGameList) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.close();

	GameData 					gd;
	std::vector<std::string> 	tags;

	// Open the test database
	ASSERT_TRUE(gd.openDatabase("/tmp/testdb.db"));
	GameDataList gdl = gd.getGameList("arcade", tags);

	GameDataItem* item = gdl.getFirst();
	ASSERT_NE(item, nullptr);
}


TEST(GameData, Metadata) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.addMetadata("1942", "arcade", "1942", "A shooting game", "~/Images/1942.png", "~/Marquees/1942.png",
				"~/Snapshots/1942.png", "~/Thumbnails/1942.png", "~/Videos/1942.wma", "1985", "Konami",
				"Konami", "Vertical Shooter", "1");
	db.close();

}
