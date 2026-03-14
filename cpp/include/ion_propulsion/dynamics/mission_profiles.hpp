// Author: A Taylor | Purpose: Orbital mechanics and mission profile calculations | Ref: Goebel/Katz/Vallado
#ifndef ION_PROPULSION_DYNAMICS_MISSION_PROFILES_HPP
#define ION_PROPULSION_DYNAMICS_MISSION_PROFILES_HPP

/**
 * @file mission_profiles.hpp
 * @brief Orbital dynamics routines for electric propulsion mission planning.
 *
 * Provides Hohmann transfer computations, Lagrange-point estimation,
 * low-thrust transfer time, and Edelbaum spiral delta-v calculations.
 */

namespace ion_propulsion::dynamics {

/**
 * @brief Result of a two-impulse Hohmann transfer.
 *
 * Each member stores one of the two velocity increments in km/s.
 */
struct TransferDeltaV {
    double delta_v1; ///< First burn delta-v at departure (km/s)
    double delta_v2; ///< Second burn delta-v at arrival  (km/s)
};

/**
 * @brief Compute Hohmann transfer delta-v from a circular parking orbit to GEO.
 *
 * @param r_park_km Parking orbit altitude above Earth's surface in kilometres.
 * @return TransferDeltaV containing both impulse magnitudes in km/s.
 *
 * The classical Hohmann transfer semi-major axis is
 * \f[
 *   a_t = \frac{r_{\text{park}} + r_{\text{GEO}}}{2}
 * \f]
 * with transfer velocities derived from the vis-viva equation.
 */
[[nodiscard]] TransferDeltaV geo_transfer_delta_v(double r_park_km);

/**
 * @brief Estimate the Sun–planet L1 Lagrange point distance.
 *
 * @param m1_kg Mass of the primary body (kg).
 * @param m2_kg Mass of the secondary body (kg).
 * @param R_m   Distance between the two bodies (m).
 * @return Approximate L1 distance from the secondary body (m).
 *
 * Uses the restricted three-body approximation:
 * \f[
 *   r_{L_1} \approx R \, \sqrt[3]{\frac{m_2}{3\,m_1}}
 * \f]
 */
[[nodiscard]] double lagrange_point_l1(double m1_kg, double m2_kg, double R_m);

/**
 * @brief Calculate low-thrust constant-acceleration transfer time.
 *
 * @param delta_v_ms Total required delta-v (m/s).
 * @param thrust_N   Continuous thrust level (N).
 * @param mass_kg    Spacecraft mass (kg).
 * @return Transfer time in seconds.
 *
 * \f[
 *   t = \frac{m \, \Delta v}{F}
 * \f]
 */
[[nodiscard]] double low_thrust_transfer_time(double delta_v_ms, double thrust_N, double mass_kg);

/**
 * @brief Edelbaum spiral delta-v between two circular orbits.
 *
 * @param r_initial_m Initial orbit radius (m).
 * @param r_final_m   Final orbit radius (m).
 * @return Delta-v in m/s.
 *
 * \f[
 *   \Delta v = \sqrt{\frac{\mu}{r_i}} - \sqrt{\frac{\mu}{r_f}}
 * \f]
 */
[[nodiscard]] double spiral_delta_v(double r_initial_m, double r_final_m);

} // namespace ion_propulsion::dynamics

#endif // ION_PROPULSION_DYNAMICS_MISSION_PROFILES_HPP
