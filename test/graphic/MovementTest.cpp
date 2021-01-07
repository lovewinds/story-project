#include "gtest/gtest.h"

#include "core/Ecore.hpp"
#include "graphic/movement/Movement.hpp"

TEST(MovementTest, SimpleCreation) {
  story::Graphic::Movement m1(10.0, 20.0, 3.14);

  EXPECT_EQ(m1.getX(), 10.0);
  EXPECT_EQ(m1.getY(), 20.0);
  EXPECT_EQ(m1.getAngle(), 3.14);
}

TEST(MovementTest, Add) {
  story::Graphic::Movement m1(10.0, 20.0, 3.14);
  story::Graphic::Movement m2(20.0, 40.0, 0.0);
  story::Graphic::Movement m3 = m1 + m2;

  EXPECT_EQ(m3.getX(), 30.0);
  EXPECT_EQ(m3.getY(), 60.0);
  EXPECT_EQ(m3.getAngle(), 3.14);
}
