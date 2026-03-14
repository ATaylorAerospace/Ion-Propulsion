// Author: A Taylor | Purpose: Unit tests for optimisation solvers | Ref: Goebel/Katz/Vallado

#include <gtest/gtest.h>
#include "ion_propulsion/optimization/solvers.hpp"
#include "ion_propulsion/constants.hpp"

#include <cmath>

using namespace ion_propulsion::optimization;
using namespace ion_propulsion::constants;

/**
 * @brief Payload fraction for Isp = 3000 s, delta-v = 4000 m/s.
 *
 * Expected: exp(-4000 / (3000 * 9.80665)) ~= 0.872
 */
TEST(OptimizationTest, PayloadFraction_Isp3000_DV4000) {
    const double pf = optimal_payload_fraction(3000.0, 4000.0);
    EXPECT_NEAR(pf, 0.872, 0.01);
}

/**
 * @brief Zero delta-v yields payload fraction of 1.
 */
TEST(OptimizationTest, PayloadFraction_ZeroDeltaV) {
    const double pf = optimal_payload_fraction(3000.0, 0.0);
    EXPECT_NEAR(pf, 1.0, 1e-12);
}

/**
 * @brief Negative Isp must throw.
 */
TEST(OptimizationTest, PayloadFraction_InvalidIsp) {
    EXPECT_THROW(optimal_payload_fraction(-1.0, 4000.0), std::invalid_argument);
}

/**
 * @brief Propellant mass for a 1000 kg spacecraft with Isp = 3000 s, dv = 4000 m/s.
 */
TEST(OptimizationTest, PropellantMass_Basic) {
    const double m0    = 1000.0;
    const double m_p   = propellant_mass(m0, 3000.0, 4000.0);
    const double pf    = optimal_payload_fraction(3000.0, 4000.0);

    // m_prop = m0 * (1 - pf)
    EXPECT_NEAR(m_p, m0 * (1.0 - pf), 1e-6);
    EXPECT_GT(m_p, 0.0);
    EXPECT_LT(m_p, m0);
}

/**
 * @brief Zero delta-v requires zero propellant.
 */
TEST(OptimizationTest, PropellantMass_ZeroDV) {
    EXPECT_NEAR(propellant_mass(1000.0, 3000.0, 0.0), 0.0, 1e-12);
}

/**
 * @brief Mission lifetime calculation.
 */
TEST(OptimizationTest, MissionLifetime_Basic) {
    const double m_prop = 100.0;    // kg
    const double m_dot  = 3.5e-6;   // kg/s

    const double t = mission_lifetime(m_prop, m_dot);
    EXPECT_NEAR(t, m_prop / m_dot, 1e-3);
}

/**
 * @brief Zero mass-flow must throw.
 */
TEST(OptimizationTest, MissionLifetime_ZeroMdot) {
    EXPECT_THROW(mission_lifetime(100.0, 0.0), std::invalid_argument);
}

/**
 * @brief Golden-section Isp optimisation must return a feasible result.
 */
TEST(OptimizationTest, OptimizeIsp_Feasible) {
    const double dv     = 5000.0;   // m/s
    const double P      = 5000.0;   // W
    const double m_tot  = 1000.0;   // kg

    const auto result = optimize_isp_for_mission(dv, P, m_tot, 0.7);

    EXPECT_GE(result.optimal_Isp, 1000.0);
    EXPECT_LE(result.optimal_Isp, 10000.0);
    EXPECT_GT(result.max_payload_fraction, 0.0);
    EXPECT_LT(result.max_payload_fraction, 1.0);
}

/**
 * @brief Higher power should allow a higher optimal Isp (or at least not lower).
 */
TEST(OptimizationTest, OptimizeIsp_HigherPower) {
    const auto r1 = optimize_isp_for_mission(5000.0, 2000.0, 1000.0);
    const auto r2 = optimize_isp_for_mission(5000.0, 10000.0, 1000.0);

    // With more power, payload fraction should be at least as good
    EXPECT_GE(r2.max_payload_fraction, r1.max_payload_fraction - 0.01);
}

/**
 * @brief Invalid power must throw.
 */
TEST(OptimizationTest, OptimizeIsp_InvalidPower) {
    EXPECT_THROW(optimize_isp_for_mission(5000.0, 0.0, 1000.0), std::invalid_argument);
}

/**
 * @brief Invalid efficiency must throw.
 */
TEST(OptimizationTest, OptimizeIsp_InvalidEfficiency) {
    EXPECT_THROW(optimize_isp_for_mission(5000.0, 5000.0, 1000.0, 0.0), std::invalid_argument);
    EXPECT_THROW(optimize_isp_for_mission(5000.0, 5000.0, 1000.0, 1.5), std::invalid_argument);
}
