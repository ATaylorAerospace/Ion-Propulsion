% Author: A Taylor | Purpose: Unit tests for orbital dynamics functions | Ref: Goebel/Katz/Vallado
classdef test_dynamics < matlab.unittest.TestCase
% TEST_DYNAMICS Unit tests for the dynamics module.
%
%   Validates GEO Hohmann transfer, Lagrange point, low-thrust transfer
%   time, and spiral delta-v against golden values shared verbatim with
%   the Python and C++ suites (1e-6 relative).

    properties (Constant)
        RelTol = 1e-6

        % Cross-language golden values
        GoldenGeo200Dv1     = 2.456667863273449     % km/s
        GoldenGeo200Dv2     = 1.478021775095608     % km/s
        GoldenL1SunEarth    = 1496417737.093085     % m
        GoldenLowThrustTime = 25000000.0            % s
        GoldenSpiralLeoGeo  = 4713.821700845472     % m/s

        MSun     = 1.989e30
        MEarth   = 5.972e24
        AU       = 1.496e11
        RLeo200  = 6.571e6
        RGeo     = 42164.0e3
        REarth   = 6.371e6
    end

    methods (Test)

        % ---- Hohmann transfer to GEO ------------------------------------
        function test_geo_transfer_golden_200km(testCase)
            [dv1, dv2] = geo_transfer_delta_v(200);
            testCase.verifyEqual(dv1, testCase.GoldenGeo200Dv1, 'RelTol', testCase.RelTol);
            testCase.verifyEqual(dv2, testCase.GoldenGeo200Dv2, 'RelTol', testCase.RelTol);
        end

        function test_geo_transfer_higher_parking_orbit_needs_less(testCase)
            [a1, a2] = geo_transfer_delta_v(200);
            [b1, b2] = geo_transfer_delta_v(1000);
            testCase.verifyLessThan(b1 + b2, a1 + a2);
        end

        function test_geo_transfer_invalid_altitude_errors(testCase)
            testCase.verifyError(@() geo_transfer_delta_v(-100), 'MATLAB:validators:mustBePositive');
        end

        % ---- Lagrange L1 ------------------------------------------------
        function test_l1_golden_sun_earth(testCase)
            r_L1 = lagrange_point_l1(testCase.MSun, testCase.MEarth, testCase.AU);
            testCase.verifyEqual(r_L1, testCase.GoldenL1SunEarth, 'RelTol', testCase.RelTol);
        end

        function test_l1_invalid_inputs_error(testCase)
            testCase.verifyError(@() lagrange_point_l1(-1, 1, 1), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() lagrange_point_l1(1, -1, 1), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() lagrange_point_l1(1, 1, 0),  'MATLAB:validators:mustBePositive');
        end

        % ---- Low-thrust transfer time -----------------------------------
        function test_low_thrust_golden(testCase)
            t = low_thrust_transfer_time(5000, 0.1, 500);
            testCase.verifyEqual(t, testCase.GoldenLowThrustTime, 'RelTol', testCase.RelTol);
        end

        function test_low_thrust_invalid_inputs_error(testCase)
            testCase.verifyError(@() low_thrust_transfer_time(5000, 0, 500), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() low_thrust_transfer_time(5000, 0.1, 0), 'MATLAB:validators:mustBePositive');
        end

        % ---- Edelbaum spiral --------------------------------------------
        function test_spiral_golden_leo_to_geo(testCase)
            dv = spiral_delta_v(testCase.RLeo200, testCase.RGeo);
            testCase.verifyEqual(dv, testCase.GoldenSpiralLeoGeo, 'RelTol', testCase.RelTol);
        end

        function test_spiral_same_radius_is_zero(testCase)
            testCase.verifyEqual(spiral_delta_v(testCase.RGeo, testCase.RGeo), 0);
        end

        function test_spiral_invalid_radii_error(testCase)
            testCase.verifyError(@() spiral_delta_v(0, testCase.RGeo),     'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() spiral_delta_v(testCase.REarth, -1),  'MATLAB:validators:mustBePositive');
        end

    end
end
