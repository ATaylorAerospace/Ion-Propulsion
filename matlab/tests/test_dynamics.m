% Author: A Taylor | Purpose: Unit tests for orbital dynamics functions | Ref: Goebel/Katz/Vallado
classdef test_dynamics < matlab.unittest.TestCase
% TEST_DYNAMICS Unit tests for the dynamics module.
%
%   Validates GEO Hohmann transfer, Lagrange point, low-thrust transfer
%   time, and spiral delta-v computations against known reference values.

    methods (Test)

        function test_geo_transfer_total_delta_v(testCase)
        % Verify combined Hohmann delta-v from 200 km parking orbit to GEO
        % is approximately 4.93 km/s (Vallado reference value).
            r_park_km = 200;  % altitude in km (consistent with Python/C++)
            [dv1, dv2] = geo_transfer_delta_v(r_park_km);
            total_dv = dv1 + dv2;
            testCase.verifyEqual(total_dv, 4.93, 'AbsTol', 0.1, ...
                'GEO Hohmann transfer delta-v should be ~4.93 km/s');
        end

        function test_geo_transfer_individual_burns(testCase)
        % Verify that both burns are positive.
            r_park_km = 200;  % altitude in km (consistent with Python/C++)
            [dv1, dv2] = geo_transfer_delta_v(r_park_km);
            testCase.verifyGreaterThan(dv1, 0, 'First burn must be positive');
            testCase.verifyGreaterThan(dv2, 0, 'Second burn must be positive');
        end

        function test_geo_transfer_first_burn_larger(testCase)
        % For LEO to GEO, the perigee burn is larger than the apogee burn.
            r_park_km = 200;  % altitude in km (consistent with Python/C++)
            [dv1, dv2] = geo_transfer_delta_v(r_park_km);
            testCase.verifyGreaterThan(dv1, dv2, ...
                'Perigee burn should exceed apogee burn for LEO-GEO');
        end

        function test_lagrange_point_l1_earth_moon(testCase)
        % Verify L1 for Earth-Moon system is a reasonable distance.
            m_earth = 5.972e24;
            m_moon  = 7.342e22;
            R_em    = 3.844e8;  % m
            r_L1 = lagrange_point_l1(m_earth, m_moon, R_em);
            testCase.verifyGreaterThan(r_L1, 0, 'L1 distance must be positive');
            testCase.verifyLessThan(r_L1, R_em, 'L1 must lie between the bodies');
        end

        function test_lagrange_point_l1_scaling(testCase)
        % L1 distance should scale with R and cube-root of mass ratio.
            r1 = lagrange_point_l1(1e6, 1e3, 1e9);
            r2 = lagrange_point_l1(1e6, 1e3, 2e9);
            testCase.verifyEqual(r2 / r1, 2.0, 'AbsTol', 1e-10, ...
                'L1 should scale linearly with R');
        end

        function test_low_thrust_transfer_time(testCase)
        % Simple consistency: t = m * dv / F
            t = low_thrust_transfer_time(5000, 0.5, 1000);
            testCase.verifyEqual(t, 1e7, 'AbsTol', 1e-6, ...
                't = 1000 * 5000 / 0.5 = 10e6 s');
        end

        function test_spiral_delta_v_positive(testCase)
        % Spiral from LEO to GEO should yield positive delta-v.
            R_earth = 6.371e6;
            r_leo = R_earth + 400e3;
            r_geo = 42164.0e3;
            dv = spiral_delta_v(r_leo, r_geo);
            testCase.verifyGreaterThan(dv, 0, ...
                'Spiral delta-v from LEO to GEO must be positive');
        end

        function test_spiral_delta_v_magnitude(testCase)
        % Spiral delta-v LEO to GEO should be a few km/s.
            R_earth = 6.371e6;
            r_leo = R_earth + 400e3;
            r_geo = 42164.0e3;
            dv = spiral_delta_v(r_leo, r_geo);
            testCase.verifyGreaterThan(dv, 3000, 'Should be > 3 km/s');
            testCase.verifyLessThan(dv, 8000, 'Should be < 8 km/s');
        end

    end
end
