% Author: A Taylor | Purpose: Unit tests for optimization module | Ref: Goebel/Katz/Vallado
classdef test_optimization < matlab.unittest.TestCase
% TEST_OPTIMIZATION Unit tests for the optimization module.
%
%   Validates payload fraction, propellant mass, mission lifetime, and
%   Isp optimization against analytic expectations.

    methods (Test)

        function test_payload_fraction_reference(testCase)
        % Payload fraction for Isp=3000 s, delta-v=4000 m/s should be ~0.872.
            frac = optimal_payload_fraction(3000, 4000);
            testCase.verifyEqual(frac, 0.872, 'AbsTol', 0.01, ...
                'Payload fraction for Isp=3000, dv=4000 should be ~0.872');
        end

        function test_payload_fraction_bounds(testCase)
        % Payload fraction must always be in (0, 1).
            frac = optimal_payload_fraction(3000, 4000);
            testCase.verifyGreaterThan(frac, 0);
            testCase.verifyLessThan(frac, 1);
        end

        function test_payload_fraction_decreases_with_dv(testCase)
        % Higher delta-v at constant Isp yields lower payload fraction.
            f1 = optimal_payload_fraction(3000, 2000);
            f2 = optimal_payload_fraction(3000, 4000);
            f3 = optimal_payload_fraction(3000, 8000);
            testCase.verifyGreaterThan(f1, f2);
            testCase.verifyGreaterThan(f2, f3);
        end

        function test_payload_fraction_increases_with_isp(testCase)
        % Higher Isp at constant delta-v yields higher payload fraction.
            f1 = optimal_payload_fraction(2000, 5000);
            f2 = optimal_payload_fraction(4000, 5000);
            testCase.verifyGreaterThan(f2, f1);
        end

        function test_propellant_mass_basic(testCase)
        % Propellant mass should equal m0 * (1 - payload_fraction).
            m0  = 1000;
            Isp = 3000;
            dv  = 4000;
            m_prop = propellant_mass(m0, Isp, dv);
            frac   = optimal_payload_fraction(Isp, dv);
            testCase.verifyEqual(m_prop, m0 * (1 - frac), 'AbsTol', 1e-6);
        end

        function test_propellant_mass_positive(testCase)
        % Propellant mass must always be positive.
            m_prop = propellant_mass(500, 3000, 5000);
            testCase.verifyGreaterThan(m_prop, 0);
        end

        function test_propellant_mass_less_than_initial(testCase)
        % Propellant mass cannot exceed initial mass.
            m0 = 1000;
            m_prop = propellant_mass(m0, 3000, 5000);
            testCase.verifyLessThan(m_prop, m0);
        end

        function test_mission_lifetime_basic(testCase)
        % t = m_prop / m_dot
            t = mission_lifetime(100, 5e-6);
            testCase.verifyEqual(t, 100 / 5e-6, 'AbsTol', 1e-6);
        end

        function test_mission_lifetime_positive(testCase)
            t = mission_lifetime(50, 1e-5);
            testCase.verifyGreaterThan(t, 0);
        end

        function test_optimize_isp_returns_valid(testCase)
        % Optimized Isp must be within the search range and payload fraction in (0,1).
            [opt_Isp, max_frac] = optimize_isp_for_mission(5000, 5000, 1000, 0.6);
            testCase.verifyGreaterThan(opt_Isp, 500);
            testCase.verifyLessThan(opt_Isp, 20000);
            testCase.verifyGreaterThan(max_frac, 0);
            testCase.verifyLessThan(max_frac, 1);
        end

        function test_optimize_isp_beats_boundary(testCase)
        % Optimized payload fraction should be at least as good as boundary values.
            dv = 5000; P = 5000; m0 = 1000; eta = 0.6;
            [~, max_frac] = optimize_isp_for_mission(dv, P, m0, eta);
            frac_low  = optimal_payload_fraction(600,  dv);
            frac_high = optimal_payload_fraction(19000, dv);
            testCase.verifyGreaterThanOrEqual(max_frac, min(frac_low, frac_high));
        end

    end
end
