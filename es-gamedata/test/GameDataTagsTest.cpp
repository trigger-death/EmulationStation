#include <gtest/gtest.h>
#include <string>
#include "GameDataTags.h"
#include "MockGameDatabase.h"

class GameDataTagsTest : public GameDataTags
{
public:
	GameDataTagsTest(sqlite3* db, std::string tagsField) : GameDataTags(db, tagsField) { }
	GameDataTagsTest(sqlite3* db, std::vector<std::string> tags) : GameDataTags(db, tags) { }

	std::string getTagID(std::string tag) { return GameDataTags::getTagID(tag); }
};


TEST(GameDataTags, GetTagID) {
	MockGameDatabase db("/tmp/testdb.db");
	db.create();

	GameDataTagsTest tags(db.mDB, "");

	std::string id1 = tags.getTagID("abc");
	std::string id2 = tags.getTagID("def");
	std::string id3 = tags.getTagID("abc");

	ASSERT_EQ(id1, "1");
	ASSERT_EQ(id2, "2");
	ASSERT_EQ(id3, "1");
}

