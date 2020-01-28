#include "gtest/gtest.h"
#include "core/Ecore.hpp"

TEST(EcoreTest, First) {
	story::Core::Ecore* core = story::Core::Ecore::getInstance();
	story::Core::Ecore* invalid = nullptr;

	EXPECT_NE(core, nullptr);
	EXPECT_NE(invalid, nullptr);

	story::Core::Ecore::releaseInstance();
}
