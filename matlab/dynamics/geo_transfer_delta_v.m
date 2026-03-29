% Author: A Taylor | Purpose: Hohmann transfer delta-v to GEO | Ref: Goebel/Katz/Vallado
function [delta_v1, delta_v2] = geo_transfer_delta_v(r_park_km)
% GEO_TRANSFER_DELTA_V Compute Hohmann transfer delta-v from parking orbit to GEO.
%
%   [delta_v1, delta_v2] = geo_transfer_delta_v(r_park_km)
%
%   Computes the two impulsive burns for a Hohmann transfer from a circular
%   parking orbit at altitude r_park_km (km above Earth's surface) to
%   geosynchronous orbit.
%
%   Inputs:
%       r_park_km - Parking orbit altitude above Earth's surface in km
%
%   Outputs:
%       delta_v1  - First burn delta-v at perigee (km/s)
%       delta_v2  - Second burn delta-v at apogee (km/s)

    arguments
        r_park_km (1,1) double {mustBePositive}
    end

    % Constants
    mu_earth = 3.986004418e14;   % m^3/s^2
    R_earth  = 6.371e6;          % m
    GEO_radius = 42164.0e3;      % m

    % Convert parking altitude (km) to orbital radius (m)
    r_park = R_earth + r_park_km * 1.0e3;
    r_geo  = GEO_radius;

    % Circular orbit velocities
    v_park = sqrt(mu_earth / r_park);
    v_geo  = sqrt(mu_earth / r_geo);

    % Transfer ellipse semi-major axis
    a_transfer = (r_park + r_geo) / 2.0;

    % Velocities on the transfer ellipse
    v_transfer_peri = sqrt(mu_earth * (2.0 / r_park - 1.0 / a_transfer));
    v_transfer_apo  = sqrt(mu_earth * (2.0 / r_geo  - 1.0 / a_transfer));

    % Delta-v for each burn, converted to km/s
    delta_v1 = (v_transfer_peri - v_park) / 1.0e3;
    delta_v2 = (v_geo - v_transfer_apo)   / 1.0e3;
end
