# 
# CMake file for gamedata/metadata library
#
project (gamedata)

set(GAMEDATA_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameData.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameDataException.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameDataList.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameDataSystem.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameDataItem.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameDataTags.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/MetaData.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PlatformId.h
)

set(GAMEDATA_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameData.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameDataException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameDataList.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameDataSystem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameDataItem.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/GameDataTags.cpp	
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MameNameMap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/MetaData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/PlatformId.cpp
)

include_directories(${COMMON_INCLUDE_DIRS})
add_library(es-gamedata STATIC ${GAMEDATA_SOURCES} ${GAMEDATA_HEADERS})
target_link_libraries(es-gamedata ${COMMON_LIBRARIES})

# If the test library is available then build and run the unit tests
if(GTEST_FOUND)
	enable_testing()

	# Build the unit test exectuable
	add_executable(
		gamedata_t
		test/TestMain.cpp
		test/GameDataTest.cpp
		test/GameDataItemTest.cpp
		test/GameDataListTest.cpp
		test/GameDataSystemTest.cpp
		test/GameDataTagsTest.cpp
		test/MockGameDatabase.h
		test/MockGameDatabase.cpp
	)
	
	target_link_libraries(
		gamedata_t
		${GTEST_BOTH_LIBRARIES}
		pthread
		es-gamedata
		es-core
		sqlite3
    	${Boost_LIBRARIES}
	)
	
	add_test(GameData gamedata_t) 

	# Make the unit tests run as part of the build
	add_custom_command(
		TARGET	gamedata_t
		COMMENT "Run tests"
		POST_BUILD COMMAND $<TARGET_FILE:gamedata_t>
	)
endif()
