---Update Set Thrust Current
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <vector.h>

#include "PPU.h"

using namespace std;

// Define the PPU class
class PPU {

public:
    PPU(int max_power, int power_supply_voltage, int power_supply_current, int thrust_voltage, int thrust_current) :
        max_power_(max_power),
        power_supply_voltage_(power_supply_voltage),
        power_supply_current_(power_supply_current),
        thrust_voltage_(thrust_voltage),
        thrust_current_(thrust_current) {
            // Initialize the PPU
        }

    void SetPowerSupplyVoltage(int voltage) {
        power_supply_voltage_ = voltage;
    }

    void SetPowerSupplyCurrent(int current) {
        power_supply_current_ = current;
    }

    void SetThrustVoltage(int voltage) {
        thrust_voltage_ = voltage;
    }

    void SetThrustCurrent(int current) {
        thrust_current_ = current;
    }

    int GetPowerSupplyVoltage() const {
        return power_supply_voltage_;
    }

    int GetPowerSupplyCurrent() const {
        return power_supply_current_;
    }

    int GetThrustVoltage() const {
        return thrust_voltage_;
    }

    int GetThrustCurrent() const {
        return thrust_current_;
    }

    void Update(int power_supply_voltage, int power_supply_current, int thrust_voltage, int thrust_current) {
        power_supply_voltage_ = power_supply_voltage;
        power_supply_current_ = power_supply_current;
        thrust_voltage_ = thrust_voltage;
        thrust_current_ = thrust_current;
    }

    int GetMaxPower() const {
        return max_power_;
    }

    void SetMaxPower(int max_power) {
        max_power_ = max_power;
    }

private:
    int max_power_;
    int power_supply_voltage_;
    int power_supply_current_;
    int thrust_voltage_;
    int thrust_current_;
};

// Define the main function
int main() {
    // Create a PPU object
    PPU ppu(1000, 12, 6, 48, 12);

    // Set the power supply voltage and current
    ppu.SetPowerSupplyVoltage(12);
    ppu.SetPowerSupplyCurrent(3);

    // Set the thrust voltage and current
    ppu.SetThrustVoltage(48);
    ppu.SetThrustCurrent(12);

    // Update the PPU with the new values
    ppu.Update(12, 6, 48, 12);

    // Get the maximum power from the PPU
    int max_power = ppu.GetMaxPower();

    // Print the maximum power
    printf("Maximum power: %d\n", max_power);

    return 0;
}
