#include "gtest/gtest.h"
#include "core/Ecore.hpp"


class GlobalEnv : public ::testing::Environment
{
public:
  GlobalEnv() { }
  ~GlobalEnv() { }

  void SetUp() {
    core = story::Core::Ecore::getInstance();
  }
  void TearDown() {
    story::Core::Ecore::releaseInstance();
    core = nullptr;
  }

  static story::Core::Ecore* core;
  static story::Core::Ecore* getCore() {
    return core;
  }
};
story::Core::Ecore* GlobalEnv::core = nullptr;


TEST(EcoreTest, First) {
  story::Core::Ecore* global_core = GlobalEnv::getCore();
  story::Core::Ecore* core = story::Core::Ecore::getInstance();

  EXPECT_NE(global_core, nullptr);
  EXPECT_EQ(global_core, core);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::AddGlobalTestEnvironment(new GlobalEnv);

  return RUN_ALL_TESTS();
}
