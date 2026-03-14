% Author: A Taylor | Purpose: Mission lifetime from propellant budget | Ref: Goebel/Katz/Vallado
function t_s = mission_lifetime(propellant_mass_kg, mass_flow_rate_kgs)
% MISSION_LIFETIME Estimate operational lifetime from propellant mass budget.
%
%   t_s = mission_lifetime(propellant_mass_kg, mass_flow_rate_kgs)
%
%   Computes the maximum mission duration given available propellant and
%   a constant mass flow rate:
%
%   $$t = \frac{m_{prop}}{\dot{m}}$$
%
%   Inputs:
%       propellant_mass_kg  - Available propellant mass (kg)
%       mass_flow_rate_kgs  - Propellant mass flow rate (kg/s)
%
%   Outputs:
%       t_s                 - Mission lifetime (s)

    arguments
        propellant_mass_kg  (1,1) double {mustBePositive}
        mass_flow_rate_kgs  (1,1) double {mustBePositive}
    end

    t_s = propellant_mass_kg / mass_flow_rate_kgs;
end
