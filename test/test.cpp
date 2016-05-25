#include "gtest/gtest.h"
#include "Ecore.hpp"

TEST(EcoreTest, First) {
	Ecore* core = Ecore::getInstance();
	Ecore* invalid = nullptr;

	EXPECT_NE(core, nullptr);
	EXPECT_NE(invalid, nullptr);

	Ecore::releaseInstance();
}
