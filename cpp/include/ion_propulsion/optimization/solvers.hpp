// Author: A Taylor | Purpose: Mission optimisation and propellant budget solvers | Ref: Goebel/Katz/Vallado
#ifndef ION_PROPULSION_OPTIMIZATION_SOLVERS_HPP
#define ION_PROPULSION_OPTIMIZATION_SOLVERS_HPP

/**
 * @file solvers.hpp
 * @brief Optimisation routines for electric propulsion mission sizing.
 *
 * Provides payload-fraction evaluation via the Tsiolkovsky equation,
 * propellant budgeting, lifetime estimation, and a golden-section search
 * for optimal specific impulse under a fixed-power constraint.
 */

namespace ion_propulsion::optimization {

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
 * @return Payload mass fraction (0–1).
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
 */
[[nodiscard]] double mission_lifetime(double propellant_mass_kg, double mass_flow_rate_kgs);

/**
 * @brief Result of an Isp optimisation sweep.
 */
struct OptimizationResult {
    double optimal_Isp;        ///< Specific impulse that maximises payload fraction (s).
    double max_payload_fraction; ///< Corresponding payload fraction.
};

/**
 * @brief Golden-section search for the Isp that maximises payload fraction.
 *
 * The search is constrained by available electrical power:
 * \f[
 *   F = \frac{2\,\eta\,P}{I_{sp}\,g_0}, \qquad
 *   \dot{m} = \frac{F}{I_{sp}\,g_0}
 * \f]
 * A candidate Isp is feasible when the resulting burn time is finite
 * and positive.
 *
 * @param delta_v_ms   Required mission delta-v (m/s).
 * @param power_W      Available electrical power (W).
 * @param total_mass_kg Total initial spacecraft mass (kg).
 * @param efficiency   Overall thruster efficiency (0–1). Default 0.7.
 * @return OptimizationResult with optimal Isp and payload fraction.
 */
[[nodiscard]] OptimizationResult optimize_isp_for_mission(double delta_v_ms,
                                                          double power_W,
                                                          double total_mass_kg,
                                                          double efficiency = 0.7);

} // namespace ion_propulsion::optimization

#endif // ION_PROPULSION_OPTIMIZATION_SOLVERS_HPP
