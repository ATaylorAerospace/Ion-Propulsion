``cpp
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

    // Test the GetPowerSupplyVoltage() method
    void testGetPowerSupplyVoltage() {
        // Assert that the power supply voltage is equal to the expected value
        CPPUNIT_ASSERT_EQUAL(ppu->GetPowerSupplyVoltage(), 12);
    }

    // Test the GetPowerSupplyCurrent() method
    void testGetPowerSupplyCurrent() {
        // Assert that the power supply current is equal to the expected value
        CPPUNIT_ASSERT_EQUAL(ppu->GetPowerSupplyCurrent(), 3);
    }

    // Test the GetThrustVoltage() method
    void testGetThrustVoltage() {
        // Assert that the thrust voltage is equal to the expected value
        CPPUNIT_ASSERT_EQUAL(ppu->GetThrustVoltage(), 48);
    }

    // Test the GetThrustCurrent() method
    void testGetThrustCurrent() {
        // Assert that the thrust current is equal to the expected value
        CPPUNIT_ASSERT_EQUAL(ppu->GetThrustCurrent(), 12);
    }

    // Test the GetMaxPower() method
    void testGetMaxPower() {
        // Assert that the maximum power is equal to the expected value
        CPPUNIT_ASSERT_EQUAL(ppu->GetMaxPower(), 1000);
    }

private:
    PPU* ppu;
};
