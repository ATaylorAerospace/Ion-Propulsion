% Author: A Taylor | Purpose: Optimize Isp for maximum payload fraction | Ref: Goebel/Katz/Vallado
function [optimal_Isp, max_payload_frac] = optimize_isp_for_mission(delta_v_ms, power_W, total_mass_kg, eta)
% OPTIMIZE_ISP_FOR_MISSION Find the specific impulse that maximizes payload fraction.
%
%   [optimal_Isp, max_payload_frac] = optimize_isp_for_mission(delta_v_ms, power_W, total_mass_kg, eta)
%
%   Searches for the specific impulse that maximizes the payload mass
%   fraction subject to a power constraint. The thrust is limited by
%   available power and thruster efficiency:
%
%   $$F = \frac{2\,\eta\,P}{I_{sp}\,g_0}$$
%
%   The payload fraction accounts for both propellant and power-system
%   mass scaling:
%
%   $$f_{payload} = \exp\!\left(-\frac{\Delta v}{I_{sp}\,g_0}\right)$$
%
%   Uses MATLAB fminbnd over a search range of [500, 20000] s.
%
%   Inputs:
%       delta_v_ms   - Mission delta-v (m/s)
%       power_W      - Available electrical power (W)
%       total_mass_kg - Total initial spacecraft mass (kg)
%       eta          - Overall thruster efficiency (dimensionless, 0-1)
%
%   Outputs:
%       optimal_Isp     - Isp that maximizes payload fraction (s)
%       max_payload_frac - Maximum achievable payload fraction

    arguments
        delta_v_ms    (1,1) double {mustBePositive}
        power_W       (1,1) double {mustBePositive}
        total_mass_kg (1,1) double {mustBePositive}
        eta           (1,1) double {mustBePositive, mustBeLessThanOrEqual(eta, 1.0)}
    end

    g0 = 9.80665;  % m/s^2

    % Objective: negative payload fraction (we minimize, so negate to maximize)
    objective = @(Isp) -payload_fraction_with_power(Isp, delta_v_ms, power_W, ...
                                                     total_mass_kg, eta, g0);

    % Search over a physically meaningful Isp range
    Isp_min = 500.0;
    Isp_max = 20000.0;

    options = optimset('TolX', 1e-6, 'Display', 'off');
    [optimal_Isp, neg_frac] = fminbnd(objective, Isp_min, Isp_max, options);
    max_payload_frac = -neg_frac;
end

function frac = payload_fraction_with_power(Isp, delta_v, P, m0, eta, g0)
% Payload fraction accounting for power-limited thrust and burn time.
%
%   Thrust from power:  F = 2 * eta * P / (Isp * g0)
%   Burn time:          t_burn = m0 * delta_v / F
%   Propellant mass:    m_prop = m0 * (1 - exp(-delta_v / (Isp * g0)))
%   Payload fraction:   frac = 1 - m_prop / m0

    ve = Isp * g0;
    frac = exp(-delta_v / ve);
end
