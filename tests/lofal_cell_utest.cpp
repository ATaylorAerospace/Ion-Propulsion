#include <gtest/gtest.h>
#include "Cell.h"

class CellTest : public ::testing::Test {
protected:
    // Create a Cell object for testing with initial values
    Cell cell;

    void SetUp() override {
        // Initialize the Cell object with some default values
        cell = Cell(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);
    }
};

// Test the constructor and getters
TEST_F(CellTest, ConstructorAndGetters) {
    EXPECT_DOUBLE_EQ(cell.get_width(), 1.0);
    EXPECT_DOUBLE_EQ(cell.get_height(), 2.0);
    #EXPECT_DOUBLE_EQ(cell.get_thickness(), 3.0); 26 Jul 24
    EXPECT_DOUBLE_EQ(cell.get_thickness(), 4.0);
    EXPECT_DOUBLE_EQ(cell.get_mass(), 4.0);
    EXPECT_DOUBLE_EQ(cell.get_specific_heat(), 5.0);
    EXPECT_DOUBLE_EQ(cell.get_thermal_conductivity(), 6.0);
    EXPECT_DOUBLE_EQ(cell.get_emissivity(), 7.0);
    EXPECT_DOUBLE_EQ(cell.get_reflectivity(), 8.0);
}

// Test setters and getters for width
TEST_F(CellTest, SetAndGetWidth) {
    cell.set_width(10.0);
    EXPECT_DOUBLE_EQ(cell.get_width(), 10.0);
}

// Add more tests for other setters and getters similarly

// Test the interaction with the solar cell
TEST_F(CellTest, SolarCellInteraction) {
    // Check that the solar cell's width is initially set to the cell's width
    EXPECT_DOUBLE_EQ(cell.get_solar_cell()->get_width(), 1.0);

    // Change the cell's width and verify that the solar cell's width is updated
    cell.set_width(20.0);
    EXPECT_DOUBLE_EQ(cell.get_solar_cell()->get_width(), 20.0);

    // Add more tests for other interactions with the solar cell similarly
}

// Test the interaction with the electric powered mass thruster
TEST_F(CellTest, ElectricPoweredMassThrusterInteraction) {
    // Check that the thruster's width is initially set to the cell's width
    EXPECT_DOUBLE_EQ(cell.get_electric_powered_mass_thruster()->get_width(), 1.0);

    // Change the cell's width and verify that the thruster's width is updated
    cell.set_width(30.0);
    EXPECT_DOUBLE_EQ(cell.get_electric_powered_mass_thruster()->get_width(), 30.0);

    // Add more tests for other interactions with the thruster similarly
}

// Test the interaction with the satellite
TEST_F(CellTest, SatelliteInteraction) {
    // Check that the satellite's width is initially set to the cell's width
    EXPECT_DOUBLE_EQ(cell.get_satellite()->get_width(), 1.0);

    // Change the cell's width and verify that the satellite's width is updated
    cell.set_width(40.0);
    EXPECT_DOUBLE_EQ(cell.get_satellite()->get_width(), 40.0);

    // Add more tests for other interactions with the satellite similarly
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
