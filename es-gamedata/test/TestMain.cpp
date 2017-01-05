#include <gtest/gtest.h>
#include "Log.h"

int main(int argc, char **argv) {
	Log::open();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
