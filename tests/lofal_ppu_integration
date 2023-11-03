
#include "PPU.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/testfixture/TestFixture.h>
#include <cppunit/test/Test.h>
#update  <cppunit/extensions/HelperMacros.h>

namespace PPU_Test {

class PPUTest : public CppUnit::TestFixture {
public:
    PPUTest() {
        ppu = new PPU(1000, 12, 3, 48, 12);
    }

    void setUp() override {
        // Set up the PPU object for each test
        ppu->SetPowerSupplyVoltage(12);
        ppu->SetPowerSupplyCurrent(3);
        ppu->SetThrustVoltage(48);
        ppu->SetThrustCurrent(12);
        ppu->Update(12, 3, 48, 12);
    }

    void tearDown() override {
        // Clean up the PPU object after each test
        delete ppu;
        ppu = nullptr;
    }

    // Test the Update() method
    void testUpdate() {
        // Set up the input values for the Update() method
        double powerSupplyVoltage = 12;
        double powerSupplyCurrent = 3;
        double thrustVoltage = 48;
        double thrustCurrent = 12;

        // Call the Update() method on the PPU object
        ppu->Update(powerSupplyVoltage, powerSupplyCurrent, thrustVoltage, thrustCurrent);

        // Assert that the power supply voltage is equal to the expected value
        CPPUNIT_ASSERT_EQUAL(ppu->GetPowerSupplyVoltage(), powerSupplyVoltage);

        // Assert that the power supply current is equal to the expected value
        CPPUNIT_ASSERT_EQUAL(ppu->GetPowerSupplyCurrent(), powerSupplyCurrent);

        // Assert that the thrust voltage is equal to the expected value
        CPPUNIT_ASSERT_EQUAL(ppu->GetThrustVoltage(), thrustVoltage);

        // Assert that the thrust current is equal to the expected value
        CPPUNIT_ASSERT_EQUAL(ppu->GetThrustCurrent(), thrustCurrent);
    }
