% Author: A Taylor | Purpose: Propellant mass from Tsiolkovsky equation | Ref: Goebel/Katz/Vallado
function m_prop = propellant_mass(m_initial_kg, Isp_s, delta_v_ms)
% PROPELLANT_MASS Propellant mass required for a given delta-v.
%
%   m_prop = propellant_mass(m_initial_kg, Isp_s, delta_v_ms)
%
%   Uses the Tsiolkovsky rocket equation to compute the propellant mass
%   consumed:
%
%   $$m_{prop} = m_0 \left(1 - \exp\!\left(-\frac{\Delta v}{I_{sp}\,g_0}\right)\right)$$
%
%   Inputs:
%       m_initial_kg - Initial spacecraft mass (kg)
%       Isp_s        - Specific impulse (s)
%       delta_v_ms   - Mission delta-v (m/s)
%
%   Outputs:
%       m_prop       - Required propellant mass (kg)

    arguments
        m_initial_kg (1,1) double {mustBePositive}
        Isp_s        (1,1) double {mustBePositive}
        delta_v_ms   (1,1) double {mustBePositive}
    end

    g0 = 9.80665;  % m/s^2
    m_prop = m_initial_kg * (1.0 - exp(-delta_v_ms / (Isp_s * g0)));
end
