// Author: A Taylor | Purpose: Mission optimisation solver implementations | Ref: Goebel/Katz/Vallado

#include "ion_propulsion/optimization/solvers.hpp"
#include "ion_propulsion/constants.hpp"

#include <cmath>
#include <stdexcept>

namespace ion_propulsion::optimization {

using namespace ion_propulsion::constants;

namespace {

constexpr int    bisection_max_iter = 200;
constexpr double bisection_tol_s    = 1.0e-9;

} // namespace

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
    return m_initial_kg * (1.0 - optimal_payload_fraction(Isp_s, delta_v_ms));
}

double mission_lifetime(double propellant_mass_kg, double mass_flow_rate_kgs) {
    if (mass_flow_rate_kgs <= 0.0) {
        throw std::invalid_argument("Mass flow rate must be positive");
    }
    return propellant_mass_kg / mass_flow_rate_kgs;
}

double power_limited_burn_time(double Isp_s,
                               double delta_v_ms,
                               double power_W,
                               double total_mass_kg,
                               double efficiency) {
    if (Isp_s <= 0.0)         throw std::invalid_argument("Specific impulse must be positive");
    if (power_W <= 0.0)       throw std::invalid_argument("Power must be positive");
    if (total_mass_kg <= 0.0) throw std::invalid_argument("Total mass must be positive");
    if (efficiency <= 0.0 || efficiency > 1.0) {
        throw std::invalid_argument("Efficiency must be in (0, 1]");
    }

    // m_dot = 2 * eta * P / ve^2 ;  t_b = m_prop / m_dot
    const double ve     = Isp_s * g0;
    const double m_prop = total_mass_kg * (1.0 - std::exp(-delta_v_ms / ve));
    const double m_dot  = 2.0 * efficiency * power_W / (ve * ve);
    return m_prop / m_dot;
}

OptimizationResult optimize_isp_for_mission(double delta_v_ms,
                                            double power_W,
                                            double total_mass_kg,
                                            double efficiency,
                                            double max_burn_time_s) {
    if (delta_v_ms <= 0.0)    throw std::invalid_argument("Delta-v must be positive");
    if (power_W <= 0.0)       throw std::invalid_argument("Power must be positive");
    if (total_mass_kg <= 0.0) throw std::invalid_argument("Total mass must be positive");
    if (efficiency <= 0.0 || efficiency > 1.0) {
        throw std::invalid_argument("Efficiency must be in (0, 1]");
    }
    if (max_burn_time_s <= 0.0) {
        throw std::invalid_argument("Maximum burn time must be positive");
    }

    const auto burn_time = [&](double isp) {
        return power_limited_burn_time(isp, delta_v_ms, power_W, total_mass_kg, efficiency);
    };

    if (burn_time(isp_search_min_s) > max_burn_time_s) {
        throw std::invalid_argument(
            "Mission infeasible: the burn time at the minimum search Isp exceeds the budget");
    }

    double best_isp = isp_search_max_s;
    if (burn_time(isp_search_max_s) > max_burn_time_s) {
        // Both payload fraction and burn time are monotonic in Isp, so the
        // optimum is the feasibility boundary: locate it by bisection.
        double lo = isp_search_min_s;
        double hi = isp_search_max_s;
        for (int i = 0; i < bisection_max_iter; ++i) {
            const double mid = 0.5 * (lo + hi);
            if (burn_time(mid) <= max_burn_time_s) {
                lo = mid;
            } else {
                hi = mid;
            }
            if (hi - lo < bisection_tol_s) break;
        }
        best_isp = lo;
    }

    return OptimizationResult{best_isp, optimal_payload_fraction(best_isp, delta_v_ms)};
}

} // namespace ion_propulsion::optimization
