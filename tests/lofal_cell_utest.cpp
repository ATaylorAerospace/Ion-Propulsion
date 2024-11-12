#include <gtest/gtest.h>
#include "Cell.h"

class CellTest : public ::testing::Test {
protected:
    Cell cell;

    void SetUp() override {
        // Initialize the Cell object with default values
        cell = Cell(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0);
    }
};

// Test the constructor and getters
TEST_F(CellTest, ConstructorAndGetters) {
    EXPECT_DOUBLE_EQ(cell.get_width(), 1.0);
    EXPECT_DOUBLE_EQ(cell.get_height(), 2.0);
    EXPECT_DOUBLE_EQ(cell.get_thickness(), 3.0);
    EXPECT_DOUBLE_EQ(cell.get_mass(), 4.0);
    EXPECT_DOUBLE_EQ(cell.get_specific_heat(), 5.0);
    EXPECT_DOUBLE_EQ(cell.get_thermal_conductivity(), 6.0);
    EXPECT_DOUBLE_EQ(cell.get_emissivity(), 7.0);
    EXPECT_DOUBLE_EQ(cell.get_reflectivity(), 8.0);
}

// Test setters and getters for all properties
TEST_F(CellTest, SettersAndGetters) {
    cell.set_width(10.0);
    EXPECT_DOUBLE_EQ(cell.get_width(), 10.0);

    cell.set_height(20.0);
    EXPECT_DOUBLE_EQ(cell.get_height(), 20.0);

    cell.set_thickness(30.0);
    EXPECT_DOUBLE_EQ(cell.get_thickness(), 30.0);

    cell.set_mass(40.0);
    EXPECT_DOUBLE_EQ(cell.get_mass(), 40.0);

    cell.set_specific_heat(50.0);
    EXPECT_DOUBLE_EQ(cell.get_specific_heat(), 50.0);

    cell.set_thermal_conductivity(60.0);
    EXPECT_DOUBLE_EQ(cell.get_thermal_conductivity(), 60.0);

    cell.set_emissivity(70.0);
    EXPECT_DOUBLE_EQ(cell.get_emissivity(), 70.0);

    cell.set_reflectivity(80.0);
    EXPECT_DOUBLE_EQ(cell.get_reflectivity(), 80.0);
}

// Test the synchronization of width with associated components
TEST_F(CellTest, ComponentWidthSynchronization) {
    // Check initial synchronization
    EXPECT_DOUBLE_EQ(cell.get_solar_cell()->get_width(), cell.get_width());
    EXPECT_DOUBLE_EQ(cell.get_electric_powered_mass_thruster()->get_width(), cell.get_width());
    EXPECT_DOUBLE_EQ(cell.get_satellite()->get_width(), cell.get_width());

    // Change the cell's width
    cell.set_width(100.0);

    // Verify that the associated components' widths are updated
    EXPECT_DOUBLE_EQ(cell.get_solar_cell()->get_width(), 100.0);
    EXPECT_DOUBLE_EQ(cell.get_electric_powered_mass_thruster()->get_width(), 100.0);
    EXPECT_DOUBLE_EQ(cell.get_satellite()->get_width(), 100.0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
