#include <gtest/gtest.h>
#include <string>
#include "GameDataItem.h"
#include "MockGameDatabase.h"

/*
 * Simple test of empty item
 */
TEST(GameDataItem, Empty) {
	GameDataItem	gdi(nullptr, "", "", "", 1, 5);
	ASSERT_TRUE(gdi.id().empty());
	ASSERT_TRUE(gdi.systemID().empty());
	ASSERT_TRUE(gdi.path().empty());
	ASSERT_EQ(gdi.rating(), 1);
	ASSERT_EQ(gdi.playCount(), 5);
}

/*
 * Test setting and retrieving data
 */
TEST(GameDataItem, Data) {
	GameDataItem	gdi(nullptr, "1942", "arcade", "1-9/1942.zip", 4, 123);
	ASSERT_EQ(gdi.id(), "1942");
	ASSERT_EQ(gdi.systemID(), "arcade");
	ASSERT_EQ(gdi.path(), "1-9/1942.zip");
	ASSERT_EQ(gdi.rating(), 4);
	ASSERT_EQ(gdi.playCount(), 123);
}

/*
 * Test tags are linked to item
 */
TEST(GameDataItem, Tags) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();
	db.addGame("rtype", "arcade", "~/Mame/rtype.zip", "3", "0");
	db.addTag("rtype", "arcade", "scroller");

	GameDataItem	gdi(db.mDB, "rtype", "arcade", "r/rtype.zip", 4, 123);
	ASSERT_TRUE(gdi.tags().hasTag("scroller"));
}
