// Author: A Taylor | Purpose: Unit tests for optimisation solvers | Ref: Goebel/Katz/Vallado

#include <gtest/gtest.h>
#include "ion_propulsion/optimization/solvers.hpp"
#include "ion_propulsion/constants.hpp"

#include <cmath>
#include <stdexcept>

using namespace ion_propulsion::optimization;
using namespace ion_propulsion::constants;

namespace {

constexpr double rel_tol = 1e-6;

/// Cross-language golden values (identical in the Python and MATLAB suites).
constexpr double golden_pf_3000s_4000ms          = 0.8728756599684215;
constexpr double golden_mprop_1000kg_3000s_4000ms = 127.12434003157847;
constexpr double golden_lifetime_100kg_3p5mgs_s  = 28571428.57142857;
constexpr double golden_burn_3000s_5kw_s         = 19325541.892783504;
// Optimiser results for dv = 5000 m/s, m0 = 1000 kg, eta = 0.7, one Julian year budget.
constexpr double golden_opt_2kw_isp_s  = 2036.9951343927255;
constexpr double golden_opt_2kw_pf     = 0.7785678565566541;
constexpr double golden_opt_5kw_isp_s  = 4751.221131411683;
constexpr double golden_opt_5kw_pf     = 0.8982463098059607;
constexpr double golden_opt_10kw_isp_s = 9260.657238489515;
constexpr double golden_opt_10kw_pf    = 0.946431802719756;

constexpr double dv  = 5000.0;
constexpr double m0  = 1000.0;
constexpr double eta = 0.7;

#define EXPECT_REL_NEAR(actual, expected) \
    EXPECT_NEAR((actual), (expected), rel_tol * std::fabs(expected))

} // anonymous namespace

// ---------------------------------------------------------------------------
// Tsiolkovsky
// ---------------------------------------------------------------------------
TEST(OptimizationTest, PayloadFraction_Golden) {
    EXPECT_REL_NEAR(optimal_payload_fraction(3000.0, 4000.0), golden_pf_3000s_4000ms);
}

TEST(OptimizationTest, PayloadFraction_ZeroDeltaVIsUnity) {
    EXPECT_DOUBLE_EQ(optimal_payload_fraction(3000.0, 0.0), 1.0);
}

TEST(OptimizationTest, PayloadFraction_Monotonic) {
    EXPECT_GT(optimal_payload_fraction(4000.0, dv), optimal_payload_fraction(2000.0, dv));
    EXPECT_GT(optimal_payload_fraction(3000.0, 2000.0), optimal_payload_fraction(3000.0, 8000.0));
}

TEST(OptimizationTest, PayloadFraction_InvalidIspThrows) {
    EXPECT_THROW((void)optimal_payload_fraction(0.0, dv), std::invalid_argument);
}

TEST(OptimizationTest, PropellantMass_Golden) {
    EXPECT_REL_NEAR(propellant_mass(1000.0, 3000.0, 4000.0), golden_mprop_1000kg_3000s_4000ms);
}

TEST(OptimizationTest, PropellantMass_ZeroDeltaV) {
    EXPECT_DOUBLE_EQ(propellant_mass(1000.0, 3000.0, 0.0), 0.0);
}

TEST(OptimizationTest, PropellantMass_InvalidMassThrows) {
    EXPECT_THROW((void)propellant_mass(0.0, 3000.0, 4000.0), std::invalid_argument);
}

TEST(OptimizationTest, MissionLifetime_Golden) {
    EXPECT_REL_NEAR(mission_lifetime(100.0, 3.5e-6), golden_lifetime_100kg_3p5mgs_s);
}

TEST(OptimizationTest, MissionLifetime_InvalidFlowThrows) {
    EXPECT_THROW((void)mission_lifetime(100.0, 0.0), std::invalid_argument);
}

// ---------------------------------------------------------------------------
// Power-limited burn time
// ---------------------------------------------------------------------------
TEST(OptimizationTest, BurnTime_Golden) {
    EXPECT_REL_NEAR(power_limited_burn_time(3000.0, dv, 5000.0, m0, eta), golden_burn_3000s_5kw_s);
}

TEST(OptimizationTest, BurnTime_IncreasesWithIsp) {
    EXPECT_GT(power_limited_burn_time(4000.0, dv, 5000.0, m0, eta),
              power_limited_burn_time(3000.0, dv, 5000.0, m0, eta));
}

TEST(OptimizationTest, BurnTime_InverselyProportionalToPower) {
    const double t5  = power_limited_burn_time(3000.0, dv, 5000.0,  m0, eta);
    const double t10 = power_limited_burn_time(3000.0, dv, 10000.0, m0, eta);
    EXPECT_REL_NEAR(t5 / t10, 2.0);
}

TEST(OptimizationTest, BurnTime_InvalidEfficiencyThrows) {
    EXPECT_THROW((void)power_limited_burn_time(3000.0, dv, 5000.0, m0, 1.5), std::invalid_argument);
}

// ---------------------------------------------------------------------------
// Isp optimiser
// ---------------------------------------------------------------------------
TEST(OptimizationTest, Optimize_Golden2kW) {
    const auto r = optimize_isp_for_mission(dv, 2000.0, m0, eta);
    EXPECT_REL_NEAR(r.optimal_Isp,          golden_opt_2kw_isp_s);
    EXPECT_REL_NEAR(r.max_payload_fraction, golden_opt_2kw_pf);
}

TEST(OptimizationTest, Optimize_Golden5kW) {
    const auto r = optimize_isp_for_mission(dv, 5000.0, m0, eta);
    EXPECT_REL_NEAR(r.optimal_Isp,          golden_opt_5kw_isp_s);
    EXPECT_REL_NEAR(r.max_payload_fraction, golden_opt_5kw_pf);
}

TEST(OptimizationTest, Optimize_Golden10kW) {
    const auto r = optimize_isp_for_mission(dv, 10000.0, m0, eta);
    EXPECT_REL_NEAR(r.optimal_Isp,          golden_opt_10kw_isp_s);
    EXPECT_REL_NEAR(r.max_payload_fraction, golden_opt_10kw_pf);
}

TEST(OptimizationTest, Optimize_BurnTimeAtOptimumMeetsBudget) {
    const auto r = optimize_isp_for_mission(dv, 5000.0, m0, eta);
    EXPECT_REL_NEAR(power_limited_burn_time(r.optimal_Isp, dv, 5000.0, m0, eta),
                    seconds_per_julian_year);
}

TEST(OptimizationTest, Optimize_MorePowerAllowsHigherIsp) {
    const auto r2  = optimize_isp_for_mission(dv, 2000.0,  m0, eta);
    const auto r10 = optimize_isp_for_mission(dv, 10000.0, m0, eta);
    EXPECT_GT(r10.optimal_Isp,          r2.optimal_Isp);
    EXPECT_GT(r10.max_payload_fraction, r2.max_payload_fraction);
}

TEST(OptimizationTest, Optimize_UnconstrainedReturnsUpperBound) {
    const auto r = optimize_isp_for_mission(dv, 1.0e6, m0, eta);
    EXPECT_DOUBLE_EQ(r.optimal_Isp, isp_search_max_s);
}

TEST(OptimizationTest, Optimize_InfeasibleBudgetThrows) {
    EXPECT_THROW((void)optimize_isp_for_mission(dv, 5000.0, m0, eta, 1.0), std::invalid_argument);
}

TEST(OptimizationTest, Optimize_InvalidInputsThrow) {
    EXPECT_THROW((void)optimize_isp_for_mission(0.0, 5000.0, m0, eta),  std::invalid_argument);
    EXPECT_THROW((void)optimize_isp_for_mission(dv,  0.0,    m0, eta),  std::invalid_argument);
    EXPECT_THROW((void)optimize_isp_for_mission(dv,  5000.0, 0.0, eta), std::invalid_argument);
    EXPECT_THROW((void)optimize_isp_for_mission(dv,  5000.0, m0, 0.0),  std::invalid_argument);
    EXPECT_THROW((void)optimize_isp_for_mission(dv,  5000.0, m0, 1.5),  std::invalid_argument);
    EXPECT_THROW((void)optimize_isp_for_mission(dv,  5000.0, m0, eta, 0.0), std::invalid_argument);
}
