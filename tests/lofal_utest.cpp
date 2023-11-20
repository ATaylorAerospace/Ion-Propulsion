#include <gtest/gtest.h>
#include "Thruster.hpp"

namespace ThrusterSpace {

// Test fixture for the Thruster class
class ThrusterTest : public ::testing::Test {
protected:
    Thruster thruster;

    void SetUp() override {
        // Initialize the Thruster object with some default values
        thruster = Thruster(1000.0, 300.0, 10.0, 0.1, 2.0, 5.0, 0.9);
    }
};

// Test GetThrust() method
TEST_F(ThrusterTest, GetThrust) {
    EXPECT_EQ(thruster.GetThrust(), 1000.0);
}

// Test GetIsp() method
TEST_F(ThrusterTest, GetIsp) {
    EXPECT_EQ(thruster.GetIsp(), 300.0);
}

// Test GetMassFlowRate() method
TEST_F(ThrusterTest, GetMassFlowRate) {
    EXPECT_EQ(thruster.GetMassFlowRate(), 10.0);
}

// Add more test cases for other methods as needed

}  // namespace ThrusterSpace

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
