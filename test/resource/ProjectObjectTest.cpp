#include "gtest/gtest.h"

#include "core/Ecore.hpp"
#include "resource/ProjectObject.hpp"

TEST(ProjectObjectTest, SimpleCreation) {
  std::shared_ptr<story::Resource::ProjectObject> obj(
    new story::Resource::ProjectObject("testType")
  );

  EXPECT_NE(obj, nullptr);
}

TEST(ProjectObjectTest, CheckType) {
  std::shared_ptr<story::Resource::ProjectObject> obj(
    new story::Resource::ProjectObject("testType")
  );

  EXPECT_EQ(obj->getType(), "testType");
}
