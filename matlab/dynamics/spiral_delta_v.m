% Author: A Taylor | Purpose: Low-thrust spiral transfer delta-v | Ref: Goebel/Katz/Vallado
function dv = spiral_delta_v(r_initial_m, r_final_m)
% SPIRAL_DELTA_V Compute delta-v for a low-thrust spiral orbit transfer.
%
%   dv = spiral_delta_v(r_initial_m, r_final_m)
%
%   For continuous low-thrust propulsion the delta-v between two circular
%   orbits is the difference in circular velocities:
%
%   $$\Delta v = \sqrt{\frac{\mu}{r_i}} - \sqrt{\frac{\mu}{r_f}}$$
%
%   Inputs:
%       r_initial_m - Initial circular orbit radius (m)
%       r_final_m   - Final circular orbit radius (m)
%
%   Outputs:
%       dv          - Delta-v magnitude (m/s)

    arguments
        r_initial_m (1,1) double {mustBePositive}
        r_final_m   (1,1) double {mustBePositive}
    end

    % Constants
    mu_earth = 3.986004418e14;  % m^3/s^2

    dv = sqrt(mu_earth / r_initial_m) - sqrt(mu_earth / r_final_m);
end
