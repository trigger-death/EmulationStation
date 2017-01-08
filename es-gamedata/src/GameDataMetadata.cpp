/*
 * GameDataMetadata.cpp
 *
 *  Created on: 7 Jan 2017
 *      Author: rhopkins
 */

#include "GameData.h"
#include "GameDataMetadata.h"
#include "GameDataException.h"
#include <sstream>

GameDataMetadata::GameDataMetadata(sqlite3* db, const std::string& id, const std::string& systemID) :
 mDB(db), mID(id), mSystemID(systemID)
{
}

GameDataMetadata::~GameDataMetadata()
{
}

std::map<std::string, std::string> GameDataMetadata::data()
{
	if (mData.empty())
	{
		sqlite3_stmt*	qresult;
		std::stringstream query;
		query << "SELECT * FROM metadata WHERE systemid='" << mSystemID << "' AND fileid='" << mID << "'";
		int result = sqlite3_prepare_v2(mDB, query.str().c_str(), query.str().size(), &qresult, NULL);
		if (result != SQLITE_OK)
		{
			std::stringstream error;
			error << "Failed to query game list folder using query: " << query << " - Error: " << error;
			throw GameDataException(error.str());
		}
		// Read all tag/value pairs and populate mData
		while (sqlite3_step(qresult) == SQLITE_ROW)
		{
			const unsigned char* tag = sqlite3_column_text(qresult, TABLE_METADATA_TAG_COL);
			const unsigned char* value = sqlite3_column_text(qresult, TABLE_METADATA_VALUE_COL);
			if (tag && value)
			{
				mData[(const char*)tag] = (const char*)value;
			}
		}
		sqlite3_finalize(qresult);
	}
	return mData;
}
