// Author: A Taylor | Purpose: Unit tests for orbital dynamics module | Ref: Goebel/Katz/Vallado

#include <gtest/gtest.h>
#include "ion_propulsion/dynamics/mission_profiles.hpp"
#include "ion_propulsion/constants.hpp"

#include <cmath>

using namespace ion_propulsion::dynamics;
using namespace ion_propulsion::constants;

/**
 * @brief Verify GEO Hohmann transfer from a 200 km parking orbit.
 *
 * Combined delta-v should be approximately 4.93 km/s, consistent with
 * standard astrodynamics references (Vallado, 4th ed.).
 */
TEST(DynamicsTest, GeoTransferDeltaV_200km) {
    const auto result = geo_transfer_delta_v(200.0);

    EXPECT_GT(result.delta_v1, 0.0);
    EXPECT_GT(result.delta_v2, 0.0);

    const double combined = result.delta_v1 + result.delta_v2;
    EXPECT_NEAR(combined, 3.93, 0.1);
}

/**
 * @brief Verify GEO transfer from a higher parking orbit produces smaller delta-v.
 */
TEST(DynamicsTest, GeoTransferDeltaV_HigherOrbitSmaller) {
    const auto low  = geo_transfer_delta_v(200.0);
    const auto high = geo_transfer_delta_v(1000.0);

    const double combined_low  = low.delta_v1  + low.delta_v2;
    const double combined_high = high.delta_v1 + high.delta_v2;

    EXPECT_LT(combined_high, combined_low);
}

/**
 * @brief Negative parking altitude must throw.
 */
TEST(DynamicsTest, GeoTransferDeltaV_InvalidAltitude) {
    EXPECT_THROW(geo_transfer_delta_v(-100.0), std::invalid_argument);
}

/**
 * @brief Sun–Earth L1 should be approximately 1.5 million km.
 */
TEST(DynamicsTest, LagrangePointL1_SunEarth) {
    constexpr double m_sun   = 1.989e30;
    constexpr double m_earth = 5.972e24;
    constexpr double au      = 1.496e11;

    const double r_l1 = lagrange_point_l1(m_sun, m_earth, au);

    // Expected ~1.5e9 m (1.5 million km)
    EXPECT_GT(r_l1, 1.0e9);
    EXPECT_LT(r_l1, 2.0e9);
}

/**
 * @brief L1 with invalid inputs must throw.
 */
TEST(DynamicsTest, LagrangePointL1_InvalidInputs) {
    EXPECT_THROW(lagrange_point_l1(-1.0, 1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(lagrange_point_l1(1.0, -1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(lagrange_point_l1(1.0, 1.0, 0.0),  std::invalid_argument);
}

/**
 * @brief Low-thrust transfer time: basic consistency check.
 */
TEST(DynamicsTest, LowThrustTransferTime) {
    const double dv   = 5000.0;  // m/s
    const double F    = 0.1;     // N
    const double mass = 500.0;   // kg

    const double t = low_thrust_transfer_time(dv, F, mass);

    // t = m * dv / F = 500 * 5000 / 0.1 = 2.5e7 s
    EXPECT_NEAR(t, 2.5e7, 1.0);
}

/**
 * @brief Low-thrust with zero thrust must throw.
 */
TEST(DynamicsTest, LowThrustTransferTime_ZeroThrust) {
    EXPECT_THROW(low_thrust_transfer_time(5000.0, 0.0, 500.0), std::invalid_argument);
}

/**
 * @brief Spiral delta-v from LEO to GEO should be positive and reasonable.
 */
TEST(DynamicsTest, SpiralDeltaV_LEOtoGEO) {
    const double r_leo = R_earth + 200.0e3;
    const double r_geo = GEO_radius;

    const double dv = spiral_delta_v(r_leo, r_geo);

    // Raising orbit => positive dv
    EXPECT_GT(dv, 0.0);
    // Should be a few km/s
    EXPECT_GT(dv, 3000.0);
    EXPECT_LT(dv, 8000.0);
}

/**
 * @brief Spiral delta-v with invalid radii must throw.
 */
TEST(DynamicsTest, SpiralDeltaV_InvalidRadii) {
    EXPECT_THROW(spiral_delta_v(0.0, GEO_radius), std::invalid_argument);
    EXPECT_THROW(spiral_delta_v(R_earth, -1.0),   std::invalid_argument);
}
