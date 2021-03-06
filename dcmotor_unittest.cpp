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

  actual = driver.drive(5.0,0.0);

  expected = 0.5;
  
  EXPECT_FLOAT_EQ(expected, actual);
  
}

TEST(DCMotorTest, symmetric2PointCurve) {

  static float x[] = {-5, 5};  //plant
  static float y[] = {-0.5, 0.5}; //drive
  static int size = 2;
  
  Driver driver = Driver(x, y, size);

  float actual = driver.drive(-5.0,0.0);

  float expected = -0.5;

  EXPECT_FLOAT_EQ(expected, actual);

  actual = driver.drive(0,0.0);

  
  expected = 0;
  
  EXPECT_FLOAT_EQ(expected, actual);
  
}
TEST(DCMotorTest, secondCurve) {

  static float x[] = {-10.0, 0.0, 5};  //plant
  static float y[] = {-0.5, 0.0, 0.5}; //drive
  static int size = 3;
  
  Driver driver = Driver(x, y, size);

  float dz = 1e-3; 
  static float x2[] = {-10.0, -8.0,    0,  -dz*2, -dz, +dz, +dz*2,  4.0,  5.0};  //plant
  static float y2[] = { -0.5, -0.4, -0.4,   -0.4,   0,  0,   0.4,  0.4,  0.5}; //drive
  static int size2 = 9;

  driver.addSecondCurve(x2,y2,size2);

  driver.threshold = 1.0;
  driver.useSecondCurveBelowThreshold;

  //expect use of second curve
  float actual = driver.drive(-1.0,0.5);
  float expected = -0.4;
  EXPECT_FLOAT_EQ(expected, actual);

  actual = driver.drive(-9,0.5);
  expected = -0.45;
  EXPECT_FLOAT_EQ(expected, actual);

  actual = driver.drive(3.0, 0.5);
  expected = 0.4;
  EXPECT_FLOAT_EQ(expected, actual);

  actual = driver.drive(4.5, 0.5);
  expected = 0.45;
  EXPECT_FLOAT_EQ(expected, actual);

  actual = driver.drive(0.0001, 0.0);
  expected = 0.0;
  EXPECT_FLOAT_EQ(expected, actual);

  actual = driver.drive(1e-12, 0.0);
  expected = 0.0;
  EXPECT_FLOAT_EQ(expected, actual);  

  actual = driver.drive(0.0, 0.0);
  expected = 0.0;
  EXPECT_FLOAT_EQ(expected, actual);
  
  //expect use of primary curve
  actual = driver.drive(-1,1.5);
  expected = -0.05;
  EXPECT_FLOAT_EQ(expected, actual);
  
  actual = driver.drive(-9,1.5);
  expected = -0.45;
  EXPECT_FLOAT_EQ(expected, actual);

  actual = driver.drive(3, 1.5);
  expected = 0.3;
  EXPECT_FLOAT_EQ(expected, actual);

  actual = driver.drive(4.5, 1.5);
  expected = 0.45;
  EXPECT_FLOAT_EQ(expected, actual);

  actual = driver.drive(0, 1.5);
  expected = 0.0;
  EXPECT_FLOAT_EQ(expected, actual); 
  
  
}

TEST(DCMotorTest, stateSpeed) {

  float SpeedMaxRPS = 20; // we can probably get to ~2500 rpm if we risk the bearings
  static float plantForSpeed[] = {-SpeedMaxRPS,SpeedMaxRPS}; //+/- 100% in the app
  static float driveForSpeed[] = {-1,1}; // max 50% drive
  static int sizeSpeed = 2;
  
  Driver driverSpeed = Driver(plantForSpeed, driveForSpeed, sizeSpeed);

  float ds = 1e-3;
  static float plantForSpeed2[] = {-SpeedMaxRPS,   -ds, 0, ds,   SpeedMaxRPS}; 
  static float driveForSpeed2[] = {-0.38,        -0.38, 0, 0.38, 0.38}; 
  static int sizeSpeed2 = 5;

  driverSpeed.addSecondCurve(plantForSpeed2, driveForSpeed2, sizeSpeed2);
  driverSpeed.threshold = 1.0; //1rps
  driverSpeed.useSecondCurveBelowThreshold = true;

  float  actual, expected;

  actual = driverSpeed.drive(10, -10);
  expected = 0.5;
  EXPECT_FLOAT_EQ(expected, actual); 
  
}

TEST(DCMotorTest, primaryOffset) {

  static float x[] = {-5, 5};  //plant
  static float y[] = {-0.5, 0.5}; //drive
  static int size = 2;
  
  Driver driver = Driver(x, y, size);
  driver.primaryOffsetPos = 0.03;
  driver.primaryOffsetNeg = -0.04;
  driver.threshold = 1.0;
  
  float actual = driver.drive(-4.0,2.0);
  float expected = -0.44;
  EXPECT_FLOAT_EQ(expected, actual);

  actual = driver.drive(3.0,2.0);
  expected = 0.33;
  EXPECT_FLOAT_EQ(expected, actual);
  
  actual = driver.drive(0,0.0);
  expected = 0;
  EXPECT_FLOAT_EQ(expected, actual);
  
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
