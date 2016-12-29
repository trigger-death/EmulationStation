#include <gtest/gtest.h>
#include <string>
#include "GameDataItem.h"

TEST(GameDataItem, Empty) {
	GameDataItem	gdi("", "", "", "", 1, 5);
	ASSERT_TRUE(gdi.id().empty());
	ASSERT_TRUE(gdi.systemID().empty());
	ASSERT_TRUE(gdi.path().empty());
	ASSERT_TRUE(gdi.tags().empty());
	ASSERT_EQ(gdi.rating(), 1);
	ASSERT_EQ(gdi.playCount(), 5);
}

TEST(GameDataItem, Data) {
	GameDataItem	gdi("1942", "arcade", "1-9/1942.zip", "vertical,shooter,favorite", 4, 123);
	ASSERT_EQ(gdi.id(), "1942");
	ASSERT_EQ(gdi.systemID(), "arcade");
	ASSERT_EQ(gdi.path(), "1-9/1942.zip");
	ASSERT_EQ(gdi.tags(), "vertical,shooter,favorite");
	ASSERT_EQ(gdi.rating(), 4);
	ASSERT_EQ(gdi.playCount(), 123);
}

