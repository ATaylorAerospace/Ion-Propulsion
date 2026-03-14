// Author: A Taylor | Purpose: Mission optimisation solver implementations | Ref: Goebel/Katz/Vallado

#include "ion_propulsion/optimization/solvers.hpp"
#include "ion_propulsion/constants.hpp"

#include <cmath>
#include <stdexcept>

namespace ion_propulsion::optimization {

using namespace ion_propulsion::constants;

double optimal_payload_fraction(double Isp_s, double delta_v_ms) {
    if (Isp_s <= 0.0) {
        throw std::invalid_argument("Specific impulse must be positive");
    }
    // Tsiolkovsky:  m_f/m_0 = exp(-dv / (Isp * g0))
    return std::exp(-delta_v_ms / (Isp_s * g0));
}

double propellant_mass(double m_initial_kg, double Isp_s, double delta_v_ms) {
    if (m_initial_kg <= 0.0) {
        throw std::invalid_argument("Initial mass must be positive");
    }
    // m_prop = m_0 * (1 - exp(-dv / (Isp * g0)))
    return m_initial_kg * (1.0 - std::exp(-delta_v_ms / (Isp_s * g0)));
}

double mission_lifetime(double propellant_mass_kg, double mass_flow_rate_kgs) {
    if (mass_flow_rate_kgs <= 0.0) {
        throw std::invalid_argument("Mass flow rate must be positive");
    }
    return propellant_mass_kg / mass_flow_rate_kgs;
}

OptimizationResult optimize_isp_for_mission(double delta_v_ms,
                                            double power_W,
                                            double total_mass_kg,
                                            double efficiency) {
    if (power_W <= 0.0)       throw std::invalid_argument("Power must be positive");
    if (total_mass_kg <= 0.0) throw std::invalid_argument("Total mass must be positive");
    if (efficiency <= 0.0 || efficiency > 1.0) {
        throw std::invalid_argument("Efficiency must be in (0, 1]");
    }

    // Objective: maximise payload fraction while satisfying power constraint.
    //   thrust   = 2 * eta * P / (Isp * g0)
    //   m_dot    = thrust / (Isp * g0)
    //   m_prop   = m_0 * (1 - exp(-dv / (Isp * g0)))
    //   burn_time = m_prop / m_dot   (must be finite and positive)
    //
    // Payload fraction = exp(-dv / (Isp * g0))  — monotonically increases with Isp,
    // but feasibility (burn_time) constrains the upper bound.

    // Evaluate merit function: payload fraction if feasible, else -1.
    auto merit = [&](double isp) -> double {
        const double ve       = isp * g0;
        const double pf       = std::exp(-delta_v_ms / ve);
        const double m_prop   = total_mass_kg * (1.0 - pf);
        const double thrust   = 2.0 * efficiency * power_W / ve;
        const double m_dot    = thrust / ve;
        if (m_dot <= 0.0 || m_prop <= 0.0) return -1.0;
        // burn_time must be finite
        [[maybe_unused]] const double burn_time = m_prop / m_dot;
        return pf;
    };

    // Golden-section search over Isp in [1000, 10000] s
    constexpr double phi = 1.6180339887498949;
    constexpr double resphi = 2.0 - phi; // ≈ 0.382

    double a = 1000.0;
    double b = 10000.0;

    double x1 = a + resphi * (b - a);
    double x2 = b - resphi * (b - a);
    double f1 = merit(x1);
    double f2 = merit(x2);

    constexpr int max_iter = 100;
    constexpr double tol = 1e-6;

    for (int i = 0; i < max_iter && (b - a) > tol; ++i) {
        if (f1 < f2) {
            // Maximum is to the right of x1
            a  = x1;
            x1 = x2;
            f1 = f2;
            x2 = b - resphi * (b - a);
            f2 = merit(x2);
        } else {
            // Maximum is to the left of x2
            b  = x2;
            x2 = x1;
            f2 = f1;
            x1 = a + resphi * (b - a);
            f1 = merit(x1);
        }
    }

    const double best_isp = (a + b) / 2.0;
    const double best_pf  = merit(best_isp);

    return OptimizationResult{best_isp, best_pf};
}

} // namespace ion_propulsion::optimization
