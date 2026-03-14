// Author: A Taylor | Purpose: Gridded ion thruster performance model | Ref: Goebel/Katz/Vallado
#ifndef ION_PROPULSION_PROPULSION_THRUSTER_HPP
#define ION_PROPULSION_PROPULSION_THRUSTER_HPP

#include "ion_propulsion/constants.hpp"

/**
 * @file thruster.hpp
 * @brief Physics-based model of a gridded ion thruster (e.g., NSTAR, NEXT).
 *
 * The model accounts for beam extraction, Child–Langmuir current limits,
 * grid erosion, and neutraliser power following the treatment in
 * Goebel & Katz, *Fundamentals of Electric Propulsion* (2008).
 */

namespace ion_propulsion::propulsion {

/**
 * @class GriddedIonThruster
 * @brief Evaluates steady-state performance of a gridded Kaufman-type ion engine.
 */
class GriddedIonThruster {
public:
    /**
     * @brief Construct a thruster model with the given operating parameters.
     *
     * @param beam_voltage          Screen grid / beam voltage \f$V_b\f$ (V).
     * @param beam_current          Total beam current \f$I_b\f$ (A).
     * @param screen_grid_voltage   Screen grid potential \f$V_s\f$ (V).
     * @param accel_grid_voltage    Accelerator grid potential \f$V_a\f$ (V, positive magnitude).
     * @param mass_flow_rate        Propellant mass flow rate \f$\dot{m}\f$ (kg/s).
     * @param propellant_mass       Single-ion mass (kg). Defaults to xenon.
     * @param grid_spacing          Screen-to-accel grid gap \f$d\f$ (m).
     * @param screen_aperture_radius  Screen aperture radius \f$r_s\f$ (m).
     * @param discharge_loss        Discharge loss per beam ion (eV/ion). Default 200 eV/ion.
     */
    GriddedIonThruster(double beam_voltage,
                       double beam_current,
                       double screen_grid_voltage,
                       double accel_grid_voltage,
                       double mass_flow_rate,
                       double propellant_mass       = constants::m_xenon,
                       double grid_spacing           = 0.001,
                       double screen_aperture_radius = 0.001,
                       double discharge_loss         = 200.0);

    /**
     * @brief Specific impulse including extraction efficiency.
     *
     * \f[
     *   I_{sp} = \frac{1}{g_0}\,\sqrt{\frac{2\,e\,V_b}{m_i}}\;\eta_{\text{ext}}
     * \f]
     * where \f$\eta_{\text{ext}} = \eta_m \, \eta_e\f$.
     *
     * @return Specific impulse in seconds.
     */
    [[nodiscard]] double specific_impulse() const;

    /**
     * @brief Thrust force.
     *
     * \f[
     *   F = \dot{m}\,I_{sp}\,g_0
     * \f]
     *
     * @return Thrust in newtons.
     */
    [[nodiscard]] double thrust() const;

    /**
     * @brief Beam electrical power.
     *
     * \f[
     *   P_b = V_b \, I_b
     * \f]
     *
     * @return Beam power in watts.
     */
    [[nodiscard]] double beam_power() const;

    /**
     * @brief Total input power (beam + discharge losses).
     *
     * \f[
     *   P_{\text{tot}} = P_b + I_b \, \varepsilon_d
     * \f]
     * where \f$\varepsilon_d\f$ is the discharge loss per beam ion (W/A ≡ eV/ion at single charge).
     *
     * @return Total power in watts.
     */
    [[nodiscard]] double total_power() const;

    /**
     * @brief Electrical efficiency.
     *
     * \f[
     *   \eta_e = \frac{P_b}{P_{\text{tot}}}
     * \f]
     *
     * @return Electrical efficiency (dimensionless, 0–1).
     */
    [[nodiscard]] double electrical_efficiency() const;

    /**
     * @brief Propellant (mass) utilisation efficiency.
     *
     * \f[
     *   \eta_m = \frac{I_b \, m_i}{e \, \dot{m}}
     * \f]
     *
     * @return Mass utilisation (dimensionless, 0–1).
     */
    [[nodiscard]] double mass_utilization() const;

    /**
     * @brief Ion extraction efficiency.
     *
     * \f[
     *   \eta_{\text{ext}} = \eta_m \, \eta_e
     * \f]
     *
     * @return Extraction efficiency (dimensionless).
     */
    [[nodiscard]] double ion_extraction_efficiency() const;

    /**
     * @brief Child–Langmuir space-charge-limited current density.
     *
     * \f[
     *   J_{CL} = \frac{4}{9}\,\varepsilon_0\,\sqrt{\frac{2\,e}{m_i}}\,
     *            \frac{V_T^{3/2}}{d^2}
     * \f]
     * where \f$V_T = V_s + V_a\f$.
     *
     * @return Current in amperes.
     */
    [[nodiscard]] double child_langmuir_current() const;

    /**
     * @brief Estimated grid erosion rate via sputtering.
     *
     * \f[
     *   \dot{m}_{\text{erode}} = Y \, \frac{I_b \, m_{\text{Mo}}}{e}
     * \f]
     * with molybdenum mass \f$m_{\text{Mo}} = 1.594 \times 10^{-25}\;\mathrm{kg}\f$.
     *
     * @param sputter_yield Sputter yield (atoms/ion). Default 0.1.
     * @return Erosion rate in kg/s.
     */
    [[nodiscard]] double grid_erosion_rate(double sputter_yield = 0.1) const;

    /**
     * @brief Neutraliser keeper power.
     *
     * \f[
     *   P_n = V_k \, I_k
     * \f]
     *
     * @param keeper_voltage Keeper discharge voltage (V). Default 20 V.
     * @param keeper_current Keeper discharge current (A). Default 2 A.
     * @return Neutraliser power in watts.
     */
    [[nodiscard]] double neutralizer_power(double keeper_voltage = 20.0,
                                           double keeper_current = 2.0) const;

private:
    double beam_voltage_;
    double beam_current_;
    double screen_grid_voltage_;
    double accel_grid_voltage_;
    double mass_flow_rate_;
    double propellant_mass_;
    double grid_spacing_;
    double screen_aperture_radius_;
    double discharge_loss_;
};

} // namespace ion_propulsion::propulsion

#endif // ION_PROPULSION_PROPULSION_THRUSTER_HPP
