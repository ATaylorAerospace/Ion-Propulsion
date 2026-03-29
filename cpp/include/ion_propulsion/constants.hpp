// Author: A Taylor | Purpose: Physical constants for ion propulsion calculations | Ref: Goebel/Katz/Vallado
#ifndef ION_PROPULSION_CONSTANTS_HPP
#define ION_PROPULSION_CONSTANTS_HPP

/**
 * @file constants.hpp
 * @brief Fundamental physical constants used throughout the ion propulsion library.
 *
 * All constants are defined as \c constexpr values for compile-time evaluation.
 * Values are consistent across C++, Python, and MATLAB implementations.
 */

namespace ion_propulsion::constants {

/// @brief Standard gravitational acceleration \f$ g_0 \f$ in \f$ \mathrm{m/s^2} \f$
inline constexpr double g0 = 9.80665;

/// @brief Earth gravitational parameter \f$ \mu_{\oplus} = GM_{\oplus} \f$ in \f$ \mathrm{m^3/s^2} \f$
inline constexpr double mu_earth = 3.986004418e14;

/// @brief Mean Earth radius \f$ R_{\oplus} \f$ in metres
inline constexpr double R_earth = 6.371e6;

/// @brief Geostationary orbit radius in metres (\f$\approx 42\,164\;\mathrm{km}\f$)
inline constexpr double GEO_radius = 42164.0e3;

/// @brief Vacuum permittivity \f$ \varepsilon_0 \f$ in \f$ \mathrm{F/m} \f$
inline constexpr double epsilon_0 = 8.854187817e-12;

/// @brief Elementary charge \f$ e \f$ in coulombs
inline constexpr double e_charge = 1.602176634e-19;

/// @brief Xenon atom mass in kg (\f$ m_{\mathrm{Xe}} \approx 131.293\;\mathrm{u} \f$)
inline constexpr double m_xenon = 2.18e-25;

} // namespace ion_propulsion::constants

#endif // ION_PROPULSION_CONSTANTS_HPP
