% Author: A Taylor | Purpose: Unit tests for propulsion module | Ref: Goebel/Katz/Vallado
classdef test_propulsion < matlab.unittest.TestCase
% TEST_PROPULSION Unit tests for the GriddedIonThruster class.
%
%   Validates the model against NSTAR flight data and against golden
%   values shared verbatim with the Python and C++ suites (1e-6 relative).

    properties (Constant)
        RelTol = 1e-6

        % NSTAR at full power: 1100 V beam, 1.76 A, 3.0 mg/s xenon, accel -180 V
        NstarFlightIsp    = 3100.0   % s
        NstarFlightThrust = 0.092    % N
        NstarFlightPower  = 2300.0   % W
        BenchmarkTol      = 0.10     % divergence and double ions not modelled
        NstarMassFlow     = 3.0e-6   % kg/s

        % Cross-language golden values
        GoldenExhaustVelocity = 40210.39047562004
        GoldenMassUtilization = 0.7982474005630351
        GoldenElectricalEff   = 0.8461538461538461
        GoldenExtractionEff   = 0.675440108168722
        GoldenIsp             = 3273.068751590841
        GoldenThrust          = 0.09629351901836497
        GoldenBeamPower       = 1936.0
        GoldenTotalPower      = 2288.0
        GoldenChildLangmuir   = 218.48574875215593
        GoldenErosion         = 1.7510179217855205e-07
        GoldenNeutralizer     = 40.0
    end

    properties
        nstar        % NSTAR full-power thruster
        highVoltage  % 1500 V operating point for the Isp >= 3000 s requirement
    end

    methods (TestMethodSetup)
        function createThrusters(testCase)
            testCase.nstar = GriddedIonThruster(1100, 1.76, 1100, -180, ...
                testCase.NstarMassFlow);
            testCase.highVoltage = GriddedIonThruster(1500, 2.0, 1100, -200, ...
                3.5e-6, 2.18e-25, 1e-3, 1e-3, 200);
        end
    end

    methods (Test)

        % ---- NSTAR benchmark --------------------------------------------
        function test_nstar_isp_within_10pct_of_flight(testCase)
            Isp = testCase.nstar.specific_impulse();
            testCase.verifyLessThan(abs(Isp - testCase.NstarFlightIsp) / testCase.NstarFlightIsp, ...
                testCase.BenchmarkTol);
        end

        function test_nstar_thrust_within_10pct_of_flight(testCase)
            F = testCase.nstar.thrust();
            testCase.verifyLessThan(abs(F - testCase.NstarFlightThrust) / testCase.NstarFlightThrust, ...
                testCase.BenchmarkTol);
        end

        function test_nstar_input_power_within_10pct_of_flight(testCase)
            P = testCase.nstar.total_power();
            testCase.verifyLessThan(abs(P - testCase.NstarFlightPower) / testCase.NstarFlightPower, ...
                testCase.BenchmarkTol);
        end

        % ---- Cross-language golden values -------------------------------
        function test_golden_efficiencies(testCase)
            t = testCase.nstar;
            testCase.verifyEqual(t.mass_utilization(),          testCase.GoldenMassUtilization, 'RelTol', testCase.RelTol);
            testCase.verifyEqual(t.electrical_efficiency(),     testCase.GoldenElectricalEff,   'RelTol', testCase.RelTol);
            testCase.verifyEqual(t.ion_extraction_efficiency(), testCase.GoldenExtractionEff,   'RelTol', testCase.RelTol);
        end

        function test_golden_exhaust_velocity_and_isp(testCase)
            t = testCase.nstar;
            testCase.verifyEqual(t.exhaust_velocity(), testCase.GoldenExhaustVelocity, 'RelTol', testCase.RelTol);
            testCase.verifyEqual(t.specific_impulse(), testCase.GoldenIsp,             'RelTol', testCase.RelTol);
        end

        function test_golden_thrust(testCase)
            testCase.verifyEqual(testCase.nstar.thrust(), testCase.GoldenThrust, 'RelTol', testCase.RelTol);
        end

        function test_golden_power(testCase)
            t = testCase.nstar;
            testCase.verifyEqual(t.beam_power(),  testCase.GoldenBeamPower,  'RelTol', testCase.RelTol);
            testCase.verifyEqual(t.total_power(), testCase.GoldenTotalPower, 'RelTol', testCase.RelTol);
        end

        function test_golden_child_langmuir(testCase)
            testCase.verifyEqual(testCase.nstar.child_langmuir_current(), ...
                testCase.GoldenChildLangmuir, 'RelTol', testCase.RelTol);
        end

        function test_golden_erosion_and_neutralizer(testCase)
            t = testCase.nstar;
            testCase.verifyEqual(t.grid_erosion_rate(), testCase.GoldenErosion,     'RelTol', testCase.RelTol);
            testCase.verifyEqual(t.neutralizer_power(), testCase.GoldenNeutralizer, 'RelTol', testCase.RelTol);
        end

        % ---- Physics consistency ----------------------------------------
        function test_high_voltage_isp_at_least_3000(testCase)
            testCase.verifyGreaterThanOrEqual(testCase.highVoltage.specific_impulse(), 3000, ...
                'Isp for a 1500 V beam must be >= 3000 s');
        end

        function test_isp_never_exceeds_ideal(testCase)
            t = testCase.highVoltage;
            testCase.verifyLessThanOrEqual(t.specific_impulse(), t.exhaust_velocity() / 9.80665);
        end

        function test_thrust_equals_eta_m_mdot_vb(testCase)
            t = testCase.nstar;
            expected = t.mass_utilization() * testCase.NstarMassFlow * t.exhaust_velocity();
            testCase.verifyEqual(t.thrust(), expected, 'RelTol', testCase.RelTol);
        end

        function test_efficiencies_bounded(testCase)
            t = testCase.nstar;
            testCase.verifyGreaterThan(t.mass_utilization(), 0);
            testCase.verifyLessThanOrEqual(t.mass_utilization(), 1);
            testCase.verifyGreaterThan(t.electrical_efficiency(), 0);
            testCase.verifyLessThan(t.electrical_efficiency(), 1);
        end

        function test_child_langmuir_sign_convention_irrelevant(testCase)
            neg = GriddedIonThruster(1100, 1.76, 1100, -180, testCase.NstarMassFlow);
            pos = GriddedIonThruster(1100, 1.76, 1100,  180, testCase.NstarMassFlow);
            testCase.verifyEqual(neg.child_langmuir_current(), pos.child_langmuir_current());
        end

        function test_child_langmuir_nonpositive_total_voltage_errors(testCase)
            t = GriddedIonThruster(1100, 1.76, -500, 100, testCase.NstarMassFlow);
            testCase.verifyError(@() t.child_langmuir_current(), ...
                'GriddedIonThruster:InvalidVoltage');
        end

        % ---- Input validation -------------------------------------------
        function test_constructor_rejects_invalid_inputs(testCase)
            testCase.verifyError(@() GriddedIonThruster(0,    1.76, 1100, -180, 3e-6), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() GriddedIonThruster(1100, 0,    1100, -180, 3e-6), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() GriddedIonThruster(1100, 1.76, 1100, -180, 0),    'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() GriddedIonThruster(1100, 1.76, 1100, -180, 3e-6, 2.18e-25, 0), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() GriddedIonThruster(1100, 1.76, 1100, -180, 3e-6, 0), 'MATLAB:validators:mustBePositive');
            testCase.verifyError(@() GriddedIonThruster(1100, 1.76, 1100, -180, 3e-6, 2.18e-25, 1e-3, 1e-3, -1), 'MATLAB:validators:mustBeNonnegative');
        end

        function test_sputter_yield_must_be_positive(testCase)
            t = testCase.nstar;
            testCase.verifyError(@() t.grid_erosion_rate(0), 'MATLAB:validators:mustBePositive');
        end

        function test_keeper_parameters_must_be_positive(testCase)
            t = testCase.nstar;
            testCase.verifyError(@() t.neutralizer_power(0, 2), 'MATLAB:validators:mustBePositive');
        end

        function test_neutralizer_custom_keeper(testCase)
            testCase.verifyEqual(testCase.nstar.neutralizer_power(15, 1.5), 22.5, 'RelTol', testCase.RelTol);
        end

        function test_default_arguments(testCase)
            t = GriddedIonThruster(1100, 1.76, 1100, -180, 3e-6);
            testCase.verifyEqual(t.propellant_mass, 2.18e-25, 'AbsTol', 1e-30);
            testCase.verifyEqual(t.grid_spacing, 1e-3);
            testCase.verifyEqual(t.discharge_loss, 200);
            testCase.verifyEqual(t.grid_erosion_rate(), t.grid_erosion_rate(0.1));
            testCase.verifyEqual(t.neutralizer_power(), t.neutralizer_power(20, 2));
        end

    end
end
