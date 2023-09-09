#include "../include/Exceptions.h"

#include "gtest/gtest.h"

// The fixture for testing class Project1. From google test primer.
class Test_Exceptions : public ::testing::Test {
protected:
  Test_Exceptions() {
    // You can do set-up work for each test here.
  }
};

TEST_F(Test_Exceptions, TestWhat) {
  try {
    throw ads::KeyNotFoundException("123");
  } catch (const std::exception &e) {
    EXPECT_STREQ(e.what(), "KeyNotFound: 123");
  }
}
