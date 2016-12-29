#include <gtest/gtest.h>
#include <string>
#include "GameDataTags.h"
#include "MockGameDatabase.h"

//
// Test we can get the tags we set
//
TEST(GameDataTags, GetTags) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();

	GameDataTags tags(db.mDB, "nes", "outrun");

	tags.addTag("abc");
	tags.addTag("def");
	tags.addTag("ghi");

	std::set<std::string> all_tags = tags.tags();

	ASSERT_EQ(all_tags.size(), 3);
	ASSERT_NE(all_tags.find("abc"), all_tags.end());
	ASSERT_NE(all_tags.find("def"), all_tags.end());
	ASSERT_NE(all_tags.find("ghi"), all_tags.end());
}

//
// Test the tags.hasTag() member
//
TEST(GameDataTags, HasTags) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();

	GameDataTags tags(db.mDB, "nes", "outrun");

	tags.addTag("abc");
	tags.addTag("def");
	tags.addTag("ghi");

	ASSERT_TRUE(tags.hasTag("abc"));
	ASSERT_TRUE(tags.hasTag("def"));
	ASSERT_TRUE(tags.hasTag("ghi"));
	ASSERT_FALSE(tags.hasTag("jkl"));
}

//
// Test the ability to remove tags
//
TEST(GameDataTags, RemoveTags) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();

	GameDataTags tags(db.mDB, "nes", "outrun");

	tags.addTag("abc");
	tags.addTag("def");
	tags.addTag("ghi");

	tags.removeTag("def");

	std::set<std::string> all_tags = tags.tags();

	ASSERT_EQ(all_tags.size(), 2);
	ASSERT_NE(all_tags.find("abc"), all_tags.end());
	ASSERT_EQ(all_tags.find("def"), all_tags.end());
	ASSERT_NE(all_tags.find("ghi"), all_tags.end());
}

//
// Test we can replace an existing set of tags with new tags
//
TEST(GameDataTags, SetTags) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();

	GameDataTags tags(db.mDB, "nes", "outrun");

	tags.addTag("abc");
	tags.addTag("def");
	tags.addTag("ghi");

	std::set<std::string> new_tags;
	new_tags.insert("one");
	new_tags.insert("two");

	tags.setTags(new_tags);

	std::set<std::string> all_tags = tags.tags();

	ASSERT_EQ(all_tags.size(), 2);
	ASSERT_NE(all_tags.find("one"), all_tags.end());
	ASSERT_NE(all_tags.find("two"), all_tags.end());
}

