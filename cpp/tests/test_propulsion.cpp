// Author: A Taylor | Purpose: Unit tests for gridded ion thruster model | Ref: Goebel/Katz/Vallado

#include <gtest/gtest.h>
#include "ion_propulsion/propulsion/thruster.hpp"
#include "ion_propulsion/constants.hpp"

#include <cmath>
#include <stdexcept>

using namespace ion_propulsion::propulsion;
using namespace ion_propulsion::constants;

namespace {

constexpr double rel_tol = 1e-6;

/// Cross-language golden values (identical in the Python and MATLAB suites).
constexpr double golden_exhaust_velocity_ms      = 40210.39047562004;
constexpr double golden_mass_utilization         = 0.7982474005630351;
constexpr double golden_electrical_efficiency    = 0.8461538461538461;
constexpr double golden_extraction_efficiency    = 0.675440108168722;
constexpr double golden_specific_impulse_s       = 3273.068751590841;
constexpr double golden_thrust_N                 = 0.09629351901836497;
constexpr double golden_beam_power_W             = 1936.0;
constexpr double golden_total_power_W            = 2288.0;
constexpr double golden_child_langmuir_Am2       = 218.48574875215593;
constexpr double golden_grid_erosion_kgs         = 1.7510179217855205e-07;
constexpr double golden_neutralizer_W            = 40.0;

/// NSTAR flight data at full power (Deep Space 1).
constexpr double nstar_flight_isp_s    = 3100.0;
constexpr double nstar_flight_thrust_N = 0.092;
constexpr double nstar_flight_power_W  = 2300.0;
constexpr double benchmark_tol         = 0.10;  // divergence and double ions not modelled

constexpr double nstar_mass_flow_kgs = 3.0e-6;

/// NSTAR at full power: 1100 V beam, 1.76 A, 3.0 mg/s xenon, accel grid at -180 V.
GriddedIonThruster make_nstar() {
    return GriddedIonThruster(1100.0, 1.76, 1100.0, -180.0, nstar_mass_flow_kgs);
}

/// 1500 V operating point used for the Isp >= 3000 s design requirement.
GriddedIonThruster make_high_voltage() {
    return GriddedIonThruster(1500.0, 1.8, 1500.0, 250.0, 3.5e-6, m_xenon, 0.7e-3, 0.95e-3);
}

#define EXPECT_REL_NEAR(actual, expected) \
    EXPECT_NEAR((actual), (expected), rel_tol * std::fabs(expected))

} // anonymous namespace

// ---------------------------------------------------------------------------
// NSTAR benchmark
// ---------------------------------------------------------------------------
TEST(PropulsionTest, Nstar_IspWithin10PercentOfFlight) {
    const double isp = make_nstar().specific_impulse();
    EXPECT_LT(std::fabs(isp - nstar_flight_isp_s) / nstar_flight_isp_s, benchmark_tol);
}

TEST(PropulsionTest, Nstar_ThrustWithin10PercentOfFlight) {
    const double F = make_nstar().thrust();
    EXPECT_LT(std::fabs(F - nstar_flight_thrust_N) / nstar_flight_thrust_N, benchmark_tol);
}

TEST(PropulsionTest, Nstar_InputPowerWithin10PercentOfFlight) {
    const double P = make_nstar().total_power();
    EXPECT_LT(std::fabs(P - nstar_flight_power_W) / nstar_flight_power_W, benchmark_tol);
}

// ---------------------------------------------------------------------------
// Cross-language golden values
// ---------------------------------------------------------------------------
TEST(PropulsionTest, Golden_Efficiencies) {
    const auto t = make_nstar();
    EXPECT_REL_NEAR(t.mass_utilization(),          golden_mass_utilization);
    EXPECT_REL_NEAR(t.electrical_efficiency(),     golden_electrical_efficiency);
    EXPECT_REL_NEAR(t.ion_extraction_efficiency(), golden_extraction_efficiency);
}

TEST(PropulsionTest, Golden_ExhaustVelocityAndIsp) {
    const auto t = make_nstar();
    EXPECT_REL_NEAR(t.exhaust_velocity(), golden_exhaust_velocity_ms);
    EXPECT_REL_NEAR(t.specific_impulse(), golden_specific_impulse_s);
}

TEST(PropulsionTest, Golden_Thrust) {
    EXPECT_REL_NEAR(make_nstar().thrust(), golden_thrust_N);
}

TEST(PropulsionTest, Golden_Power) {
    const auto t = make_nstar();
    EXPECT_REL_NEAR(t.beam_power(),  golden_beam_power_W);
    EXPECT_REL_NEAR(t.total_power(), golden_total_power_W);
}

TEST(PropulsionTest, Golden_ChildLangmuir) {
    EXPECT_REL_NEAR(make_nstar().child_langmuir_current(), golden_child_langmuir_Am2);
}

TEST(PropulsionTest, Golden_ErosionAndNeutralizer) {
    const auto t = make_nstar();
    EXPECT_REL_NEAR(t.grid_erosion_rate(), golden_grid_erosion_kgs);
    EXPECT_REL_NEAR(t.neutralizer_power(), golden_neutralizer_W);
}

// ---------------------------------------------------------------------------
// Physics consistency
// ---------------------------------------------------------------------------
TEST(PropulsionTest, HighVoltage_IspAtLeast3000s) {
    EXPECT_GE(make_high_voltage().specific_impulse(), 3000.0);
}

TEST(PropulsionTest, Isp_NeverExceedsIdeal) {
    const auto t = make_high_voltage();
    EXPECT_LE(t.specific_impulse(), t.exhaust_velocity() / g0);
}

TEST(PropulsionTest, Thrust_EqualsEtaMassFlowExhaustVelocity) {
    const auto t = make_nstar();
    const double expected = t.mass_utilization() * nstar_mass_flow_kgs * t.exhaust_velocity();
    EXPECT_REL_NEAR(t.thrust(), expected);
}

TEST(PropulsionTest, Efficiencies_Bounded) {
    const auto t = make_nstar();
    EXPECT_GT(t.mass_utilization(), 0.0);
    EXPECT_LE(t.mass_utilization(), 1.0);
    EXPECT_GT(t.electrical_efficiency(), 0.0);
    EXPECT_LT(t.electrical_efficiency(), 1.0);
}

TEST(PropulsionTest, ChildLangmuir_AccelSignConventionIrrelevant) {
    const GriddedIonThruster neg(1100.0, 1.76, 1100.0, -180.0, nstar_mass_flow_kgs);
    const GriddedIonThruster pos(1100.0, 1.76, 1100.0,  180.0, nstar_mass_flow_kgs);
    EXPECT_DOUBLE_EQ(neg.child_langmuir_current(), pos.child_langmuir_current());
}

TEST(PropulsionTest, ChildLangmuir_NonPositiveTotalVoltageThrows) {
    const GriddedIonThruster t(1100.0, 1.76, -500.0, 100.0, nstar_mass_flow_kgs);
    EXPECT_THROW((void)t.child_langmuir_current(), std::invalid_argument);
}

// ---------------------------------------------------------------------------
// Input validation
// ---------------------------------------------------------------------------
TEST(PropulsionTest, Constructor_RejectsInvalidInputs) {
    EXPECT_THROW(GriddedIonThruster(0.0,    1.76, 1100.0, -180.0, 3.0e-6), std::invalid_argument);
    EXPECT_THROW(GriddedIonThruster(1100.0, 0.0,  1100.0, -180.0, 3.0e-6), std::invalid_argument);
    EXPECT_THROW(GriddedIonThruster(1100.0, 1.76, 1100.0, -180.0, 0.0),    std::invalid_argument);
    EXPECT_THROW(GriddedIonThruster(1100.0, 1.76, 1100.0, -180.0, 3.0e-6, m_xenon, 0.0), std::invalid_argument);
    EXPECT_THROW(GriddedIonThruster(1100.0, 1.76, 1100.0, -180.0, 3.0e-6, 0.0),     std::invalid_argument);
    EXPECT_THROW(GriddedIonThruster(1100.0, 1.76, 1100.0, -180.0, 3.0e-6, m_xenon, 1e-3, 1e-3, -1.0),
                 std::invalid_argument);
}

TEST(PropulsionTest, SputterYield_MustBePositive) {
    EXPECT_THROW((void)make_nstar().grid_erosion_rate(0.0), std::invalid_argument);
}

TEST(PropulsionTest, KeeperParameters_MustBePositive) {
    EXPECT_THROW((void)make_nstar().neutralizer_power(0.0, 2.0), std::invalid_argument);
}

TEST(PropulsionTest, Neutralizer_CustomKeeper) {
    EXPECT_REL_NEAR(make_nstar().neutralizer_power(15.0, 1.5), 22.5);
}
