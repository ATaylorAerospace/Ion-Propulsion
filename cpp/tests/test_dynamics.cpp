// Author: A Taylor | Purpose: Unit tests for orbital dynamics module | Ref: Goebel/Katz/Vallado

#include <gtest/gtest.h>
#include "ion_propulsion/dynamics/mission_profiles.hpp"
#include "ion_propulsion/constants.hpp"

#include <cmath>
#include <stdexcept>

using namespace ion_propulsion::dynamics;
using namespace ion_propulsion::constants;

namespace {

constexpr double rel_tol = 1e-6;

/// Cross-language golden values (identical in the Python and MATLAB suites).
constexpr double golden_geo200_dv1_kms       = 2.456667863273449;
constexpr double golden_geo200_dv2_kms       = 1.478021775095608;
constexpr double golden_l1_sun_earth_m       = 1496417737.093085;
constexpr double golden_low_thrust_t_s       = 25000000.0;
constexpr double golden_spiral_leo200_geo_ms = 4713.821700845472;

constexpr double m_sun   = 1.989e30;
constexpr double m_earth = 5.972e24;
constexpr double au_m    = 1.496e11;
constexpr double r_leo_200km_m = 6.571e6;

#define EXPECT_REL_NEAR(actual, expected) \
    EXPECT_NEAR((actual), (expected), rel_tol * std::fabs(expected))

} // anonymous namespace

// ---------------------------------------------------------------------------
// Hohmann transfer to GEO
// ---------------------------------------------------------------------------
TEST(DynamicsTest, GeoTransfer_Golden200km) {
    const auto r = geo_transfer_delta_v(200.0);
    EXPECT_REL_NEAR(r.delta_v1, golden_geo200_dv1_kms);
    EXPECT_REL_NEAR(r.delta_v2, golden_geo200_dv2_kms);
}

TEST(DynamicsTest, GeoTransfer_HigherParkingOrbitNeedsLess) {
    const auto low  = geo_transfer_delta_v(200.0);
    const auto high = geo_transfer_delta_v(1000.0);
    EXPECT_LT(high.delta_v1 + high.delta_v2, low.delta_v1 + low.delta_v2);
}

TEST(DynamicsTest, GeoTransfer_InvalidAltitudeThrows) {
    EXPECT_THROW((void)geo_transfer_delta_v(-100.0), std::invalid_argument);
}

// ---------------------------------------------------------------------------
// Lagrange L1
// ---------------------------------------------------------------------------
TEST(DynamicsTest, L1_GoldenSunEarth) {
    EXPECT_REL_NEAR(lagrange_point_l1(m_sun, m_earth, au_m), golden_l1_sun_earth_m);
}

TEST(DynamicsTest, L1_InvalidInputsThrow) {
    EXPECT_THROW((void)lagrange_point_l1(-1.0, 1.0, 1.0), std::invalid_argument);
    EXPECT_THROW((void)lagrange_point_l1(1.0, -1.0, 1.0), std::invalid_argument);
    EXPECT_THROW((void)lagrange_point_l1(1.0, 1.0, 0.0),  std::invalid_argument);
}

// ---------------------------------------------------------------------------
// Low-thrust transfer time
// ---------------------------------------------------------------------------
TEST(DynamicsTest, LowThrust_Golden) {
    EXPECT_REL_NEAR(low_thrust_transfer_time(5000.0, 0.1, 500.0), golden_low_thrust_t_s);
}

TEST(DynamicsTest, LowThrust_InvalidInputsThrow) {
    EXPECT_THROW((void)low_thrust_transfer_time(5000.0, 0.0, 500.0), std::invalid_argument);
    EXPECT_THROW((void)low_thrust_transfer_time(5000.0, 0.1, 0.0),   std::invalid_argument);
}

// ---------------------------------------------------------------------------
// Edelbaum spiral
// ---------------------------------------------------------------------------
TEST(DynamicsTest, Spiral_GoldenLeoToGeo) {
    EXPECT_REL_NEAR(spiral_delta_v(r_leo_200km_m, GEO_radius), golden_spiral_leo200_geo_ms);
}

TEST(DynamicsTest, Spiral_SameRadiusIsZero) {
    EXPECT_DOUBLE_EQ(spiral_delta_v(GEO_radius, GEO_radius), 0.0);
}

TEST(DynamicsTest, Spiral_InvalidRadiiThrow) {
    EXPECT_THROW((void)spiral_delta_v(0.0, GEO_radius), std::invalid_argument);
    EXPECT_THROW((void)spiral_delta_v(R_earth, -1.0),   std::invalid_argument);
}
