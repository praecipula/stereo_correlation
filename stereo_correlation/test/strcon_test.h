#include "strcon.h"
#include <QPoint>
#include "gtest/gtest.h"

using namespace Stereo;
using namespace std;

// The fixture for testing class Foo.
class StrConTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  StrConTest() {
    // You can do set-up work for each test here.
  }

  virtual ~StrConTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(StrConTest, ConvertsPoints) {
    EXPECT_EQ(string("10x15"), string(StrCon(QPoint(10, 15))));
}
