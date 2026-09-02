% Author: A Taylor | Purpose: Burn time for a delta-v at fixed electrical power | Ref: Goebel/Katz/Vallado
function t_b = power_limited_burn_time(Isp_s, delta_v_ms, power_W, total_mass_kg, eta)
% POWER_LIMITED_BURN_TIME Burn time needed to deliver a delta-v at fixed power.
%
%   t_b = power_limited_burn_time(Isp_s, delta_v_ms, power_W, total_mass_kg, eta)
%
%   With jet power $\eta P = \tfrac{1}{2}\dot{m} v_e^2$ (Goebel and Katz
%   eq. 2.3-9) and $v_e = I_{sp} g_0$, the mass flow rate available at a
%   given specific impulse is
%
%   $$\dot{m} = \frac{2\,\eta\,P}{v_e^2}, \qquad F = \frac{2\,\eta\,P}{v_e}$$
%
%   and the burn time follows from the Tsiolkovsky propellant mass:
%
%   $$t_b = \frac{m_{prop}}{\dot{m}}
%         = \frac{m_0\left(1 - e^{-\Delta v / v_e}\right) v_e^2}{2\,\eta\,P}$$
%
%   Inputs:
%       Isp_s         - Specific impulse (s)
%       delta_v_ms    - Mission delta-v (m/s)
%       power_W       - Electrical power delivered to the thruster (W)
%       total_mass_kg - Initial spacecraft mass (kg)
%       eta           - Total thruster efficiency (0-1), default 0.7
%
%   Outputs:
%       t_b           - Burn time (s)

    arguments
        Isp_s         (1,1) double {mustBePositive}
        delta_v_ms    (1,1) double {mustBeNonnegative}
        power_W       (1,1) double {mustBePositive}
        total_mass_kg (1,1) double {mustBePositive}
        eta           (1,1) double {mustBePositive, mustBeLessThanOrEqual(eta, 1.0)} = 0.7
    end

    g0 = 9.80665;  % m/s^2

    ve     = Isp_s * g0;
    m_prop = total_mass_kg * (1.0 - exp(-delta_v_ms / ve));
    m_dot  = 2.0 * eta * power_W / (ve * ve);
    t_b    = m_prop / m_dot;
end
