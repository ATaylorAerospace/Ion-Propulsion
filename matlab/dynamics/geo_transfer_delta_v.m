% Author: A Taylor | Purpose: Hohmann transfer delta-v to GEO | Ref: Goebel/Katz/Vallado
function [delta_v1, delta_v2] = geo_transfer_delta_v(r_park_km)
% GEO_TRANSFER_DELTA_V Compute Hohmann transfer delta-v from parking orbit to GEO.
%
%   [delta_v1, delta_v2] = geo_transfer_delta_v(r_park_km)
%
%   Computes the two impulsive burns for a Hohmann transfer from a circular
%   parking orbit of radius r_park_km (km) to geosynchronous orbit.
%
%   The Hohmann transfer ellipse has semi-major axis:
%
%   $$a_{transfer} = \frac{r_{park} + r_{GEO}}{2}$$
%
%   The perigee and apogee velocities on the transfer ellipse are:
%
%   $$v_{t,peri} = \sqrt{\mu \left(\frac{2}{r_{park}} - \frac{1}{a_{transfer}}\right)}$$
%   $$v_{t,apo}  = \sqrt{\mu \left(\frac{2}{r_{GEO}}  - \frac{1}{a_{transfer}}\right)}$$
%
%   Inputs:
%       r_park_km - Parking orbit radius in km (scalar, positive)
%
%   Outputs:
%       delta_v1  - First burn delta-v at perigee (km/s)
%       delta_v2  - Second burn delta-v at apogee (km/s)

    arguments
        r_park_km (1,1) double {mustBePositive}
    end

    % Constants
    mu_earth = 3.986004418e14;   % m^3/s^2
    GEO_radius = 42164.0e3;     % m

    % Convert parking orbit radius to meters
    r_park = r_park_km * 1.0e3;
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
