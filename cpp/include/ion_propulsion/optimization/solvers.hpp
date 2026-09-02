// Author: A Taylor | Purpose: Mission optimisation and propellant budget solvers | Ref: Goebel/Katz/Vallado
#ifndef ION_PROPULSION_OPTIMIZATION_SOLVERS_HPP
#define ION_PROPULSION_OPTIMIZATION_SOLVERS_HPP

#include "ion_propulsion/constants.hpp"

/**
 * @file solvers.hpp
 * @brief Optimisation routines for electric propulsion mission sizing.
 *
 * Provides payload-fraction evaluation via the Tsiolkovsky equation,
 * propellant budgeting, lifetime estimation, power-limited burn time, and a
 * bisection search for the optimal specific impulse under a fixed power
 * budget and a maximum burn time.
 */

namespace ion_propulsion::optimization {

/// @brief Lower bound of the specific impulse search window (s), shared across languages.
inline constexpr double isp_search_min_s = 500.0;
/// @brief Upper bound of the specific impulse search window (s), shared across languages.
inline constexpr double isp_search_max_s = 20000.0;

/**
 * @brief Compute the optimal (maximum) payload mass fraction.
 *
 * From the Tsiolkovsky rocket equation:
 * \f[
 *   \frac{m_f}{m_0} = \exp\!\left(-\frac{\Delta v}{I_{sp}\,g_0}\right)
 * \f]
 *
 * @param Isp_s      Specific impulse (s).
 * @param delta_v_ms Required delta-v (m/s).
 * @return Payload mass fraction (0 to 1).
 * @throws std::invalid_argument if the specific impulse is not positive.
 */
[[nodiscard]] double optimal_payload_fraction(double Isp_s, double delta_v_ms);

/**
 * @brief Required propellant mass for a given delta-v.
 *
 * \f[
 *   m_p = m_0 \left(1 - \exp\!\left(-\frac{\Delta v}{I_{sp}\,g_0}\right)\right)
 * \f]
 *
 * @param m_initial_kg Initial spacecraft mass (kg).
 * @param Isp_s        Specific impulse (s).
 * @param delta_v_ms   Required delta-v (m/s).
 * @return Propellant mass (kg).
 * @throws std::invalid_argument if the initial mass or specific impulse is not positive.
 */
[[nodiscard]] double propellant_mass(double m_initial_kg, double Isp_s, double delta_v_ms);

/**
 * @brief Mission lifetime limited by propellant supply.
 *
 * \f[
 *   t = \frac{m_p}{\dot{m}}
 * \f]
 *
 * @param propellant_mass_kg  Available propellant (kg).
 * @param mass_flow_rate_kgs  Propellant consumption rate (kg/s).
 * @return Mission lifetime (s).
 * @throws std::invalid_argument if the mass flow rate is not positive.
 */
[[nodiscard]] double mission_lifetime(double propellant_mass_kg, double mass_flow_rate_kgs);

/**
 * @brief Burn time needed to deliver a delta-v with a fixed electrical power.
 *
 * With jet power \f$\eta P = \tfrac{1}{2}\dot{m} v_e^2\f$ (Goebel & Katz
 * eq. 2.3-9) and \f$v_e = I_{sp} g_0\f$:
 * \f[
 *   \dot{m} = \frac{2\,\eta\,P}{v_e^2}, \qquad
 *   t_b = \frac{m_p}{\dot{m}}
 *       = \frac{m_0\left(1 - e^{-\Delta v / v_e}\right) v_e^2}{2\,\eta\,P}
 * \f]
 *
 * @param Isp_s         Specific impulse (s).
 * @param delta_v_ms    Mission delta-v (m/s).
 * @param power_W       Electrical power delivered to the thruster (W).
 * @param total_mass_kg Initial spacecraft mass (kg).
 * @param efficiency    Total thruster efficiency (0 to 1). Default 0.7.
 * @return Burn time (s).
 * @throws std::invalid_argument if any input is out of range.
 */
[[nodiscard]] double power_limited_burn_time(double Isp_s,
                                             double delta_v_ms,
                                             double power_W,
                                             double total_mass_kg,
                                             double efficiency = 0.7);

/**
 * @brief Result of an Isp optimisation.
 */
struct OptimizationResult {
    double optimal_Isp;          ///< Specific impulse that maximises payload fraction (s).
    double max_payload_fraction; ///< Corresponding payload fraction.
};

/**
 * @brief Find the Isp that maximises payload fraction under a power budget
 *        and a maximum burn time.
 *
 * The payload fraction \f$e^{-\Delta v/(I_{sp} g_0)}\f$ and the
 * power-limited burn time both increase monotonically with \f$I_{sp}\f$, so
 * the optimum is the largest \f$I_{sp}\f$ in \f$[500, 20000]\f$ s whose burn
 * time fits the budget:
 * \f[
 *   I_{sp}^{\ast} = \max\{\,I_{sp} : t_b(I_{sp}) \le t_{max}\,\}
 * \f]
 * Located by bisection so that every language implementation agrees to well
 * within \f$10^{-6}\f$ relative tolerance.
 *
 * @param delta_v_ms      Required mission delta-v (m/s), positive.
 * @param power_W         Available electrical power (W).
 * @param total_mass_kg   Total initial spacecraft mass (kg).
 * @param efficiency      Total thruster efficiency (0 to 1). Default 0.7.
 * @param max_burn_time_s Maximum allowed burn time (s). Default one Julian year.
 * @return OptimizationResult with optimal Isp and payload fraction.
 * @throws std::invalid_argument if an input is out of range, or if the mission
 *         is infeasible even at the minimum search Isp.
 */
[[nodiscard]] OptimizationResult optimize_isp_for_mission(
    double delta_v_ms,
    double power_W,
    double total_mass_kg,
    double efficiency      = 0.7,
    double max_burn_time_s = constants::seconds_per_julian_year);

} // namespace ion_propulsion::optimization

#endif // ION_PROPULSION_OPTIMIZATION_SOLVERS_HPP
