// Author: A Taylor | Purpose: Orbital mechanics and mission profile implementations | Ref: Goebel/Katz/Vallado

#include "ion_propulsion/dynamics/mission_profiles.hpp"
#include "ion_propulsion/constants.hpp"

#include <cmath>
#include <stdexcept>

namespace ion_propulsion::dynamics {

using namespace ion_propulsion::constants;

TransferDeltaV geo_transfer_delta_v(double r_park_km) {
    if (r_park_km <= 0.0) {
        throw std::invalid_argument("Parking orbit altitude must be positive");
    }

    // Convert parking altitude (km) to orbital radius (m)
    const double r_park = r_park_km * 1000.0 + R_earth;
    const double r_geo  = GEO_radius;

    // Circular velocity at parking orbit
    const double v_park = std::sqrt(mu_earth / r_park);

    // Hohmann transfer ellipse semi-major axis
    const double a_transfer = (r_park + r_geo) / 2.0;

    // Transfer orbit velocities via vis-viva:  v = sqrt(mu * (2/r - 1/a))
    const double v_transfer_peri = std::sqrt(mu_earth * (2.0 / r_park - 1.0 / a_transfer));
    const double v_transfer_apo  = std::sqrt(mu_earth * (2.0 / r_geo  - 1.0 / a_transfer));

    // Circular velocity at GEO
    const double v_geo = std::sqrt(mu_earth / r_geo);

    // Delta-v for each burn, converted to km/s
    const double dv1 = (v_transfer_peri - v_park) / 1000.0;
    const double dv2 = (v_geo - v_transfer_apo)   / 1000.0;

    return TransferDeltaV{dv1, dv2};
}

double lagrange_point_l1(double m1_kg, double m2_kg, double R_m) {
    if (m1_kg <= 0.0 || m2_kg <= 0.0 || R_m <= 0.0) {
        throw std::invalid_argument("Masses and distance must be positive");
    }

    // L1 approximation:  r_L1 = R * (m2 / (3 * m1))^(1/3)
    return R_m * std::cbrt(m2_kg / (3.0 * m1_kg));
}

double low_thrust_transfer_time(double delta_v_ms, double thrust_N, double mass_kg) {
    if (thrust_N <= 0.0) {
        throw std::invalid_argument("Thrust must be positive");
    }
    if (mass_kg <= 0.0) {
        throw std::invalid_argument("Mass must be positive");
    }

    // t = m * dv / F
    return mass_kg * delta_v_ms / thrust_N;
}

double spiral_delta_v(double r_initial_m, double r_final_m) {
    if (r_initial_m <= 0.0 || r_final_m <= 0.0) {
        throw std::invalid_argument("Orbital radii must be positive");
    }

    // Edelbaum spiral:  dv = sqrt(mu/r_i) - sqrt(mu/r_f)
    return std::sqrt(mu_earth / r_initial_m) - std::sqrt(mu_earth / r_final_m);
}

} // namespace ion_propulsion::dynamics
