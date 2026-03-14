% Author: A Taylor | Purpose: Low-thrust transfer time estimate | Ref: Goebel/Katz/Vallado
function t_s = low_thrust_transfer_time(delta_v_ms, thrust_N, mass_kg)
% LOW_THRUST_TRANSFER_TIME Estimate transfer time for a low-thrust maneuver.
%
%   t_s = low_thrust_transfer_time(delta_v_ms, thrust_N, mass_kg)
%
%   Computes the time required for a constant-thrust, constant-mass
%   approximation of a low-thrust transfer:
%
%   $$t = \frac{m \cdot \Delta v}{F}$$
%
%   Inputs:
%       delta_v_ms - Required delta-v (m/s)
%       thrust_N   - Continuous thrust magnitude (N)
%       mass_kg    - Spacecraft mass (kg)
%
%   Outputs:
%       t_s        - Transfer time (s)

    arguments
        delta_v_ms (1,1) double {mustBePositive}
        thrust_N   (1,1) double {mustBePositive}
        mass_kg    (1,1) double {mustBePositive}
    end

    t_s = mass_kg * delta_v_ms / thrust_N;
end
