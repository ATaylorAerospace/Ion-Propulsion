// Author: A Taylor | Purpose: Gridded ion thruster performance implementation | Ref: Goebel/Katz/Vallado

#include "ion_propulsion/propulsion/thruster.hpp"
#include "ion_propulsion/constants.hpp"

#include <cmath>
#include <stdexcept>

namespace ion_propulsion::propulsion {

using namespace ion_propulsion::constants;

GriddedIonThruster::GriddedIonThruster(double beam_voltage,
                                       double beam_current,
                                       double screen_grid_voltage,
                                       double accel_grid_voltage,
                                       double mass_flow_rate,
                                       double propellant_mass,
                                       double grid_spacing,
                                       double screen_aperture_radius,
                                       double discharge_loss)
    : beam_voltage_{beam_voltage}
    , beam_current_{beam_current}
    , screen_grid_voltage_{screen_grid_voltage}
    , accel_grid_voltage_{accel_grid_voltage}
    , mass_flow_rate_{mass_flow_rate}
    , propellant_mass_{propellant_mass}
    , grid_spacing_{grid_spacing}
    , screen_aperture_radius_{screen_aperture_radius}
    , discharge_loss_{discharge_loss}
{
    if (beam_voltage_ <= 0.0)    throw std::invalid_argument("Beam voltage must be positive");
    if (beam_current_ <= 0.0)    throw std::invalid_argument("Beam current must be positive");
    if (mass_flow_rate_ <= 0.0)  throw std::invalid_argument("Mass flow rate must be positive");
    if (grid_spacing_ <= 0.0)    throw std::invalid_argument("Grid spacing must be positive");
    if (propellant_mass_ <= 0.0) throw std::invalid_argument("Propellant mass must be positive");
    if (discharge_loss_ < 0.0)   throw std::invalid_argument("Discharge loss must be non-negative");
}

double GriddedIonThruster::exhaust_velocity() const {
    // v_b = sqrt(2 * e * V_beam / m_ion)
    return std::sqrt(2.0 * e_charge * beam_voltage_ / propellant_mass_);
}

double GriddedIonThruster::specific_impulse() const {
    // Isp = eta_m * v_b / g0   (Goebel & Katz eq. 2.4-8, unit divergence)
    return mass_utilization() * exhaust_velocity() / g0;
}

double GriddedIonThruster::thrust() const {
    // F = m_dot * Isp * g0
    return mass_flow_rate_ * specific_impulse() * g0;
}

double GriddedIonThruster::beam_power() const {
    // P_beam = V_beam * I_beam
    return beam_voltage_ * beam_current_;
}

double GriddedIonThruster::total_power() const {
    // P_total = P_beam + I_beam * discharge_loss
    return beam_power() + beam_current_ * discharge_loss_;
}

double GriddedIonThruster::electrical_efficiency() const {
    // eta_e = P_beam / P_total
    return beam_power() / total_power();
}

double GriddedIonThruster::mass_utilization() const {
    // eta_m = (I_beam * m_ion) / (e * m_dot)
    return (beam_current_ * propellant_mass_) / (e_charge * mass_flow_rate_);
}

double GriddedIonThruster::ion_extraction_efficiency() const {
    // eta_ext = eta_e * eta_m
    return electrical_efficiency() * mass_utilization();
}

double GriddedIonThruster::child_langmuir_current() const {
    // J_CL = (4/9) * eps0 * sqrt(2*e/m_ion) * V_total^(3/2) / d^2,  V_total = V_screen + |V_accel|
    const double V_total = screen_grid_voltage_ + std::fabs(accel_grid_voltage_);
    if (V_total <= 0.0) {
        throw std::invalid_argument("Total extraction voltage (screen + |accel|) must be positive");
    }
    return (4.0 / 9.0) * epsilon_0
           * std::sqrt(2.0 * e_charge / propellant_mass_)
           * std::pow(V_total, 1.5)
           / (grid_spacing_ * grid_spacing_);
}

double GriddedIonThruster::grid_erosion_rate(double sputter_yield) const {
    if (sputter_yield <= 0.0) {
        throw std::invalid_argument("Sputter yield must be positive");
    }
    // erosion = Y * I_beam * m_Mo / e
    return sputter_yield * beam_current_ * m_molybdenum / e_charge;
}

double GriddedIonThruster::neutralizer_power(double keeper_voltage,
                                              double keeper_current) const {
    if (keeper_voltage <= 0.0 || keeper_current <= 0.0) {
        throw std::invalid_argument("Keeper voltage and current must be positive");
    }
    // P_neut = V_keeper * I_keeper
    return keeper_voltage * keeper_current;
}

} // namespace ion_propulsion::propulsion
