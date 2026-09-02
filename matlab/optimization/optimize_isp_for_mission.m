% Author: A Taylor | Purpose: Optimize Isp for maximum payload fraction | Ref: Goebel/Katz/Vallado
function [optimal_Isp, max_payload_frac] = optimize_isp_for_mission(delta_v_ms, power_W, total_mass_kg, eta, max_burn_time_s)
% OPTIMIZE_ISP_FOR_MISSION Isp that maximizes payload fraction at fixed power.
%
%   [optimal_Isp, max_payload_frac] = optimize_isp_for_mission(delta_v_ms, ...
%       power_W, total_mass_kg, eta, max_burn_time_s)
%
%   The payload fraction $f = \exp(-\Delta v / (I_{sp} g_0))$ increases
%   monotonically with Isp, but at fixed power a higher Isp lowers the mass
%   flow rate and lengthens the burn (see POWER_LIMITED_BURN_TIME), which
%   also increases monotonically. The optimum is therefore the largest Isp
%   in the search window [500, 20000] s whose burn time fits the budget:
%
%   $$I_{sp}^{*} = \max\{\, I_{sp} : t_b(I_{sp}) \le t_{max} \,\}$$
%
%   It is located by bisection so that the Python, C++, and MATLAB
%   implementations agree to well within 1e-6 relative tolerance.
%
%   Inputs:
%       delta_v_ms      - Mission delta-v (m/s), positive
%       power_W         - Available electrical power (W)
%       total_mass_kg   - Total initial spacecraft mass (kg)
%       eta             - Total thruster efficiency (0-1), default 0.7
%       max_burn_time_s - Maximum allowed burn time (s), default one
%                         Julian year (31557600 s)
%
%   Outputs:
%       optimal_Isp      - Isp that maximizes payload fraction (s)
%       max_payload_frac - Payload fraction at the optimal Isp
%
%   Errors if the mission is infeasible even at the minimum search Isp.

    arguments
        delta_v_ms      (1,1) double {mustBePositive}
        power_W         (1,1) double {mustBePositive}
        total_mass_kg   (1,1) double {mustBePositive}
        eta             (1,1) double {mustBePositive, mustBeLessThanOrEqual(eta, 1.0)} = 0.7
        max_burn_time_s (1,1) double {mustBePositive} = 31557600.0
    end

    % Search window and bisection settings shared by every language implementation
    Isp_min  = 500.0;
    Isp_max  = 20000.0;
    max_iter = 200;
    tol_s    = 1.0e-9;

    burn_time = @(Isp) power_limited_burn_time(Isp, delta_v_ms, power_W, total_mass_kg, eta);

    if burn_time(Isp_min) > max_burn_time_s
        error('optimize_isp_for_mission:Infeasible', ...
            ['Mission infeasible: the burn time at the minimum search Isp ', ...
             '(%.0f s) exceeds the %.4g s budget'], Isp_min, max_burn_time_s);
    end

    if burn_time(Isp_max) <= max_burn_time_s
        best_Isp = Isp_max;
    else
        lo = Isp_min;
        hi = Isp_max;
        for k = 1:max_iter
            mid = 0.5 * (lo + hi);
            if burn_time(mid) <= max_burn_time_s
                lo = mid;
            else
                hi = mid;
            end
            if (hi - lo) < tol_s
                break;
            end
        end
        best_Isp = lo;
    end

    optimal_Isp      = best_Isp;
    max_payload_frac = optimal_payload_fraction(best_Isp, delta_v_ms);
end
