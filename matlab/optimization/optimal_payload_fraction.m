% Author: A Taylor | Purpose: Optimal payload mass fraction via Tsiolkovsky | Ref: Goebel/Katz/Vallado
function frac = optimal_payload_fraction(Isp_s, delta_v_ms)
% OPTIMAL_PAYLOAD_FRACTION Payload fraction from the rocket equation.
%
%   frac = optimal_payload_fraction(Isp_s, delta_v_ms)
%
%   Computes the payload mass fraction achievable for a given specific
%   impulse and mission delta-v using the Tsiolkovsky rocket equation:
%
%   $$f_{payload} = \exp\!\left(-\frac{\Delta v}{I_{sp}\,g_0}\right)$$
%
%   Inputs:
%       Isp_s      - Specific impulse (s)
%       delta_v_ms - Mission delta-v (m/s)
%
%   Outputs:
%       frac       - Payload mass fraction (dimensionless)

    arguments
        Isp_s      (1,1) double {mustBePositive}
        delta_v_ms (1,1) double {mustBePositive}
    end

    g0 = 9.80665;  % m/s^2
    frac = exp(-delta_v_ms / (Isp_s * g0));
end
