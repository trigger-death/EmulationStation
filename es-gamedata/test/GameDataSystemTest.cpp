#include <gtest/gtest.h>
#include <string>
#include "GameDataSystem.h"
#include "PlatformId.h"

TEST(GameDataSystem, ID)
{
	GameDataSystem	system("arcade", "");
	EXPECT_EQ(system.id(), "arcade");
}

TEST(GameDataSystem, rootPath)
{
	GameDataSystem	system("arcade", "/home/pi/mame");
	EXPECT_EQ(system.getRootPath(), "/home/pi/mame");
}

TEST(GameDataSystem, hasPlatformId)
{
	GameDataSystem system("arcade", "");
	EXPECT_TRUE(system.hasPlatformId(PlatformIds::ARCADE));
	EXPECT_FALSE(system.hasPlatformId(PlatformIds::AMSTRAD_CPC));
}
