//pid_unittest.cpp

#include "dcmotor.cpp"
#include <gtest/gtest.h>

TEST(DCMotorTest, primaryCurve) {

  static float x[] = {-10.0, 0.0, 5};  //plant
  static float y[] = {-0.5, 0.0, 0.5}; //drive
  static int size = 3;
  
  Driver driver = Driver(x, y, size);

  float actual = driver.drive(-5.0,0.0);

  float expected = -0.25;

  EXPECT_FLOAT_EQ(expected, actual);
 
  
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
