% Author: A Taylor | Purpose: Unit tests for optimization module | Ref: Goebel/Katz/Vallado
classdef test_optimization < matlab.unittest.TestCase
% TEST_OPTIMIZATION Unit tests for the optimization module.
%
%   Validates payload fraction, propellant mass, mission lifetime,
%   power-limited burn time, and the Isp optimizer against golden values
%   shared verbatim with the Python and C++ suites (1e-6 relative).

    properties (Constant)
        RelTol = 1e-6
        SecondsPerJulianYear = 31557600.0
        IspSearchMax = 20000.0

        % Cross-language golden values
        GoldenPf3000s4000ms         = 0.8728756599684215
        GoldenMprop1000kg3000s4000ms = 127.12434003157847
        GoldenLifetime100kg3p5mgs   = 28571428.57142857
        GoldenBurn3000s5kW          = 19325541.892783504
        % Optimizer results for dv = 5000 m/s, m0 = 1000 kg, eta = 0.7, one year budget
        GoldenOpt2kWIsp  = 2036.9951343927255
        GoldenOpt2kWPf   = 0.7785678565566541
        GoldenOpt5kWIsp  = 4751.221131411683
        GoldenOpt5kWPf   = 0.8982463098059607
        GoldenOpt10kWIsp = 9260.657238489515
        GoldenOpt10kWPf  = 0.946431802719756

        Dv  = 5000
        M0  = 1000
        Eta = 0.7
    end

    methods (Test)

        % ---- Tsiolkovsky --------------------------------------------------
        function test_payload_fraction_golden(testCase)
            testCase.verifyEqual(optimal_payload_fraction(3000, 4000), ...
                testCase.GoldenPf3000s4000ms, 'RelTol', testCase.RelTol);
        end

        function test_payload_fraction_zero_delta_v_is_unity(testCase)
            testCase.verifyEqual(optimal_payload_fraction(3000, 0), 1);
        end

        function test_payload_fraction_monotonic(testCase)
            testCase.verifyGreaterThan(optimal_payload_fraction(4000, testCase.Dv), ...
                optimal_payload_fraction(2000, testCase.Dv));
            testCase.verifyGreaterThan(optimal_payload_fraction(3000, 2000), ...
                optimal_payload_fraction(3000, 8000));
        end

        function test_payload_fraction_invalid_isp_errors(testCase)
            testCase.verifyError(@() optimal_payload_fraction(0, testCase.Dv), ...
                'MATLAB:validators:mustBePositive');
        end

        function test_propellant_mass_golden(testCase)
            testCase.verifyEqual(propellant_mass(1000, 3000, 4000), ...
                testCase.GoldenMprop1000kg3000s4000ms, 'RelTol', testCase.RelTol);
        end

        function test_propellant_mass_zero_delta_v(testCase)
            testCase.verifyEqual(propellant_mass(1000, 3000, 0), 0);
        end

        function test_propellant_mass_invalid_mass_errors(testCase)
            testCase.verifyError(@() propellant_mass(0, 3000, 4000), ...
                'MATLAB:validators:mustBePositive');
        end

        function test_mission_lifetime_golden(testCase)
            testCase.verifyEqual(mission_lifetime(100, 3.5e-6), ...
                testCase.GoldenLifetime100kg3p5mgs, 'RelTol', testCase.RelTol);
        end

        function test_mission_lifetime_invalid_flow_errors(testCase)
            testCase.verifyError(@() mission_lifetime(100, 0), ...
                'MATLAB:validators:mustBePositive');
        end

        % ---- Power-limited burn time -------------------------------------
        function test_burn_time_golden(testCase)
            t_b = power_limited_burn_time(3000, testCase.Dv, 5000, testCase.M0, testCase.Eta);
            testCase.verifyEqual(t_b, testCase.GoldenBurn3000s5kW, 'RelTol', testCase.RelTol);
        end

        function test_burn_time_increases_with_isp(testCase)
            testCase.verifyGreaterThan( ...
                power_limited_burn_time(4000, testCase.Dv, 5000, testCase.M0, testCase.Eta), ...
                power_limited_burn_time(3000, testCase.Dv, 5000, testCase.M0, testCase.Eta));
        end

        function test_burn_time_inversely_proportional_to_power(testCase)
            t5  = power_limited_burn_time(3000, testCase.Dv, 5000,  testCase.M0, testCase.Eta);
            t10 = power_limited_burn_time(3000, testCase.Dv, 10000, testCase.M0, testCase.Eta);
            testCase.verifyEqual(t5 / t10, 2, 'RelTol', testCase.RelTol);
        end

        function test_burn_time_invalid_efficiency_errors(testCase)
            testCase.verifyError(@() power_limited_burn_time(3000, testCase.Dv, 5000, testCase.M0, 1.5), ...
                'MATLAB:validators:mustBeLessThanOrEqual');
        end

        % ---- Isp optimizer ------------------------------------------------
        function test_optimize_golden_2kW(testCase)
            [Isp, pf] = optimize_isp_for_mission(testCase.Dv, 2000, testCase.M0, testCase.Eta);
            testCase.verifyEqual(Isp, testCase.GoldenOpt2kWIsp, 'RelTol', testCase.RelTol);
            testCase.verifyEqual(pf,  testCase.GoldenOpt2kWPf,  'RelTol', testCase.RelTol);
        end

        function test_optimize_golden_5kW(testCase)
            [Isp, pf] = optimize_isp_for_mission(testCase.Dv, 5000, testCase.M0, testCase.Eta);
            testCase.verifyEqual(Isp, testCase.GoldenOpt5kWIsp, 'RelTol', testCase.RelTol);
            testCase.verifyEqual(pf,  testCase.GoldenOpt5kWPf,  'RelTol', testCase.RelTol);
        end

        function test_optimize_golden_10kW(testCase)
            [Isp, pf] = optimize_isp_for_mission(testCase.Dv, 10000, testCase.M0, testCase.Eta);
            testCase.verifyEqual(Isp, testCase.GoldenOpt10kWIsp, 'RelTol', testCase.RelTol);
            testCase.verifyEqual(pf,  testCase.GoldenOpt10kWPf,  'RelTol', testCase.RelTol);
        end

        function test_optimize_burn_time_at_optimum_meets_budget(testCase)
            Isp = optimize_isp_for_mission(testCase.Dv, 5000, testCase.M0, testCase.Eta);
            t_b = power_limited_burn_time(Isp, testCase.Dv, 5000, testCase.M0, testCase.Eta);
            testCase.verifyEqual(t_b, testCase.SecondsPerJulianYear, 'RelTol', testCase.RelTol);
        end

        function test_optimize_more_power_allows_higher_isp(testCase)
            [Isp2,  pf2]  = optimize_isp_for_mission(testCase.Dv, 2000,  testCase.M0, testCase.Eta);
            [Isp10, pf10] = optimize_isp_for_mission(testCase.Dv, 10000, testCase.M0, testCase.Eta);
            testCase.verifyGreaterThan(Isp10, Isp2);
            testCase.verifyGreaterThan(pf10, pf2);
        end

        function test_optimize_unconstrained_returns_upper_bound(testCase)
            Isp = optimize_isp_for_mission(testCase.Dv, 1e6, testCase.M0, testCase.Eta);
            testCase.verifyEqual(Isp, testCase.IspSearchMax);
        end

        function test_optimize_infeasible_budget_errors(testCase)
            testCase.verifyError(@() optimize_isp_for_mission(testCase.Dv, 5000, testCase.M0, testCase.Eta, 1), ...
                'optimize_isp_for_mission:Infeasible');
        end

        function test_optimize_invalid_inputs_error(testCase)
            testCase.verifyError(@() optimize_isp_for_mission(0, 5000, testCase.M0, testCase.Eta), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() optimize_isp_for_mission(testCase.Dv, 0, testCase.M0, testCase.Eta), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() optimize_isp_for_mission(testCase.Dv, 5000, 0, testCase.Eta), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() optimize_isp_for_mission(testCase.Dv, 5000, testCase.M0, 0), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() optimize_isp_for_mission(testCase.Dv, 5000, testCase.M0, 1.5), 'MATLAB:validators:mustBeLessThanOrEqual');
            testCase.verifyError(@() optimize_isp_for_mission(testCase.Dv, 5000, testCase.M0, testCase.Eta, 0), 'MATLAB:validators:mustBePositive');
        end

        function test_optimize_default_arguments(testCase)
            [Isp_default, pf_default] = optimize_isp_for_mission(testCase.Dv, 5000, testCase.M0);
            [Isp_explicit, pf_explicit] = optimize_isp_for_mission(testCase.Dv, 5000, testCase.M0, 0.7, 31557600);
            testCase.verifyEqual(Isp_default, Isp_explicit);
            testCase.verifyEqual(pf_default, pf_explicit);
        end

    end
end
