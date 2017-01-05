#include <gtest/gtest.h>
#include <string>
#include "GameData.h"
#include "GameDataItem.h"
#include "GameDataFolder.h"
#include "MockGameDatabase.h"

TEST(GameData, OpenDatabase) {
	GameData	gd;
	ASSERT_TRUE(gd.openDatabase("/tmp/testdb.db"));
}

TEST(GameData, GetGameList) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("1942", "arcade", "~/Mame/1942.zip", "", "3", "0");
	db.close();

	GameData 					gd;

	// Open the test database
	ASSERT_TRUE(gd.openDatabase("/tmp/testdb.db"));
	GameDataList gdl = gd.getGameList();

	ASSERT_NE(gdl.folder()->items().size(), 0);
}

TEST(GameData, AddNewGame)
{
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.close();

	// Create dummy games
	mkdir("/tmp/es-games", S_IRWXU);
	mkdir("/tmp/es-games/mame", S_IRWXU);
	FILE* fp = fopen("/tmp/es-games/mame/abc.zip", "wb");
	fclose(fp);

	GameData gd;
	gd.openDatabase("/tmp/testdb.db");
	GameDataSystem system("arcade", "/tmp/es-games/mame");

	EXPECT_NO_THROW(gd.addGame(system, "/tmp/es-games/mame/abc.zip"));
}

TEST(GameData, AddExistingGame)
{
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.close();

	// Create dummy games
	mkdir("/tmp/es-games", S_IRWXU);
	mkdir("/tmp/es-games/mame", S_IRWXU);
	mkdir("/tmp/es-games/mame/A", S_IRWXU);
	FILE* fp = fopen("/tmp/es-games/mame/A/abc.zip", "wb");
	fclose(fp);

	GameData gd;
	gd.openDatabase("/tmp/testdb.db");
	GameDataSystem system("arcade", "/tmp/es-games/mame");

	EXPECT_NO_THROW(gd.addGame(system, "/tmp/es-games/mame/A/abc.zip"));
	EXPECT_NO_THROW(gd.addGame(system, "/tmp/es-games/mame/A/abc.zip"));
}

TEST(GameData, ParseGameList)
{
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.close();

	GameData gd;
	gd.openDatabase("/tmp/testdb.db");
	GameDataSystem system("mame", "/home/rhopkins/src/EmulationStation/FullConfig/Mame-0.37b5");

	gd.parseGameList(system, "/home/rhopkins/.emulationstation/gamelists/mame/gamelist.xml");
}

/*
 * Parse a game list file when the games already exist in the database
 */
TEST(GameData, ParseGameListGamesExist)
{
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.close();

	GameData gd;
	gd.openDatabase("/tmp/testdb.db");
	GameDataSystem system("mame", "/home/rhopkins/src/EmulationStation/FullConfig/Mame-0.37b5");

	gd.parseGameList(system, "/home/rhopkins/.emulationstation/gamelists/mame/gamelist.xml");
	gd.parseGameList(system, "/home/rhopkins/.emulationstation/gamelists/mame/gamelist.xml");
}
