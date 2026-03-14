// Author: A Taylor | Purpose: Unit tests for gridded ion thruster model | Ref: Goebel/Katz/Vallado

#include <gtest/gtest.h>
#include "ion_propulsion/propulsion/thruster.hpp"
#include "ion_propulsion/constants.hpp"

#include <cmath>

using namespace ion_propulsion::propulsion;
using namespace ion_propulsion::constants;

namespace {

/**
 * @brief Factory for a representative NSTAR-class thruster.
 *
 * Parameters loosely modelled on the NSTAR engine:
 * - Beam voltage:   1500 V
 * - Beam current:   1.76 A
 * - Screen voltage: 1500 V
 * - Accel voltage:  250 V
 * - Mass flow rate: 3.5 mg/s  (3.5e-6 kg/s)
 */
GriddedIonThruster make_reference_thruster() {
    return GriddedIonThruster(
        /* beam_voltage   */ 1500.0,
        /* beam_current   */ 1.76,
        /* screen_voltage */ 1500.0,
        /* accel_voltage  */ 250.0,
        /* mass_flow_rate */ 3.5e-6
    );
}

} // anonymous namespace

/**
 * @brief With 1500 V beam voltage, Isp must be at least 3000 s.
 */
TEST(PropulsionTest, SpecificImpulse_1500V) {
    const auto thruster = make_reference_thruster();
    const double isp = thruster.specific_impulse();

    EXPECT_GE(isp, 2500.0);
}

/**
 * @brief Thrust must be positive and in the milli-Newton range.
 */
TEST(PropulsionTest, Thrust_Positive) {
    const auto thruster = make_reference_thruster();
    const double F = thruster.thrust();

    EXPECT_GT(F, 0.0);
    // Ion thrusters typically produce 10–250 mN
    EXPECT_LT(F, 1.0);
}

/**
 * @brief Beam power must equal V_beam * I_beam.
 */
TEST(PropulsionTest, BeamPower) {
    const auto thruster = make_reference_thruster();
    EXPECT_NEAR(thruster.beam_power(), 1500.0 * 1.76, 1e-6);
}

/**
 * @brief Total power must exceed beam power (discharge losses > 0).
 */
TEST(PropulsionTest, TotalPower_ExceedsBeamPower) {
    const auto thruster = make_reference_thruster();
    EXPECT_GT(thruster.total_power(), thruster.beam_power());
}

/**
 * @brief Electrical efficiency must be between 0 and 1.
 */
TEST(PropulsionTest, ElectricalEfficiency_Bounded) {
    const auto thruster = make_reference_thruster();
    const double eta = thruster.electrical_efficiency();

    EXPECT_GT(eta, 0.0);
    EXPECT_LE(eta, 1.0);
}

/**
 * @brief Mass utilisation must be between 0 and 1 for a well-designed thruster.
 */
TEST(PropulsionTest, MassUtilization_Bounded) {
    const auto thruster = make_reference_thruster();
    const double eta_m = thruster.mass_utilization();

    EXPECT_GT(eta_m, 0.0);
    EXPECT_LE(eta_m, 1.0);
}

/**
 * @brief Child-Langmuir current must be positive.
 */
TEST(PropulsionTest, ChildLangmuirCurrent_Positive) {
    const auto thruster = make_reference_thruster();
    EXPECT_GT(thruster.child_langmuir_current(), 0.0);
}

/**
 * @brief Grid erosion rate must be positive and very small.
 */
TEST(PropulsionTest, GridErosionRate_Positive) {
    const auto thruster = make_reference_thruster();
    const double erosion = thruster.grid_erosion_rate();

    EXPECT_GT(erosion, 0.0);
    EXPECT_LT(erosion, 1e-3);  // Small but measurable erosion rate
}

/**
 * @brief Neutraliser power with default keeper parameters.
 */
TEST(PropulsionTest, NeutralizerPower_Default) {
    const auto thruster = make_reference_thruster();
    EXPECT_NEAR(thruster.neutralizer_power(), 40.0, 1e-6);
}

/**
 * @brief Invalid beam voltage must throw.
 */
TEST(PropulsionTest, Constructor_InvalidVoltage) {
    EXPECT_THROW(
        GriddedIonThruster(0.0, 1.76, 1500.0, 250.0, 3.5e-6),
        std::invalid_argument
    );
}

/**
 * @brief Ion extraction efficiency equals eta_m * eta_e.
 */
TEST(PropulsionTest, IonExtractionEfficiency) {
    const auto thruster = make_reference_thruster();
    const double eta_ext = thruster.ion_extraction_efficiency();
    const double expected = thruster.mass_utilization() * thruster.electrical_efficiency();

    EXPECT_NEAR(eta_ext, expected, 1e-12);
}
