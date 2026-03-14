% Author: A Taylor | Purpose: Unit tests for propulsion module | Ref: Goebel/Katz/Vallado
classdef test_propulsion < matlab.unittest.TestCase
% TEST_PROPULSION Unit tests for the GriddedIonThruster class.
%
%   Validates thruster performance metrics against physical expectations
%   and known reference values for typical ion thruster parameters.

    properties
        thruster  % Default test thruster instance
    end

    methods (TestMethodSetup)
        function createThruster(testCase)
        % Create a reference thruster for test methods.
        %   V_beam = 1500 V, I_beam = 2 A, V_screen = 1100 V,
        %   V_accel = -200 V, m_dot = 5e-6 kg/s, m_ion = 2.18e-25 kg,
        %   grid_spacing = 1e-3 m, aperture_radius = 1e-3 m,
        %   discharge_loss = 200 W/A.
            testCase.thruster = GriddedIonThruster(1500, 2.0, 1100, -200, ...
                5e-6, 2.18e-25, 1e-3, 1e-3, 200);
        end
    end

    methods (Test)

        function test_specific_impulse_above_3000(testCase)
        % A 1500 V beam thruster should achieve Isp >= 3000 s.
            Isp = testCase.thruster.specific_impulse();
            testCase.verifyGreaterThanOrEqual(Isp, 3000, ...
                'Isp for V_beam=1500V must be >= 3000 s');
        end

        function test_specific_impulse_reasonable_range(testCase)
        % Isp should fall within physically realistic bounds for xenon.
            Isp = testCase.thruster.specific_impulse();
            testCase.verifyGreaterThan(Isp, 1000, 'Isp too low');
            testCase.verifyLessThan(Isp, 10000, 'Isp too high');
        end

        function test_beam_power(testCase)
        % Beam power should equal V_beam * I_beam.
            P = testCase.thruster.beam_power();
            testCase.verifyEqual(P, 3000, 'AbsTol', 1e-10, ...
                'Beam power should be 1500 * 2 = 3000 W');
        end

        function test_total_power_exceeds_beam(testCase)
        % Total power must exceed beam power due to discharge losses.
            P_beam  = testCase.thruster.beam_power();
            P_total = testCase.thruster.total_power();
            testCase.verifyGreaterThan(P_total, P_beam, ...
                'Total power must exceed beam power');
        end

        function test_total_power_value(testCase)
        % P_total = P_beam + I_beam * discharge_loss = 3000 + 2*200 = 3400 W
            P_total = testCase.thruster.total_power();
            testCase.verifyEqual(P_total, 3400, 'AbsTol', 1e-10, ...
                'Total power should be 3400 W');
        end

        function test_electrical_efficiency(testCase)
        % Electrical efficiency must be between 0 and 1.
            eta_e = testCase.thruster.electrical_efficiency();
            testCase.verifyGreaterThan(eta_e, 0, 'eta_e must be > 0');
            testCase.verifyLessThanOrEqual(eta_e, 1, 'eta_e must be <= 1');
        end

        function test_electrical_efficiency_value(testCase)
        % eta_e = 3000 / 3400 = 15/17
            eta_e = testCase.thruster.electrical_efficiency();
            testCase.verifyEqual(eta_e, 3000/3400, 'AbsTol', 1e-10);
        end

        function test_mass_utilization(testCase)
        % Mass utilization must be between 0 and 1 for valid operation.
            eta_m = testCase.thruster.mass_utilization();
            testCase.verifyGreaterThan(eta_m, 0, 'eta_m must be > 0');
            testCase.verifyLessThanOrEqual(eta_m, 1, 'eta_m must be <= 1');
        end

        function test_ion_extraction_efficiency(testCase)
        % Extraction efficiency = eta_e * eta_m, must be in (0, 1].
            eta_ext = testCase.thruster.ion_extraction_efficiency();
            testCase.verifyGreaterThan(eta_ext, 0);
            testCase.verifyLessThanOrEqual(eta_ext, 1);
        end

        function test_thrust_positive(testCase)
        % Thrust must be positive.
            F = testCase.thruster.thrust();
            testCase.verifyGreaterThan(F, 0, 'Thrust must be positive');
        end

        function test_child_langmuir_current_positive(testCase)
        % Child-Langmuir current density must be positive.
            J_CL = testCase.thruster.child_langmuir_current();
            testCase.verifyGreaterThan(J_CL, 0, 'J_CL must be positive');
        end

        function test_grid_erosion_rate(testCase)
        % Grid erosion rate must be positive for a positive sputter yield.
            erosion = testCase.thruster.grid_erosion_rate(0.1);
            testCase.verifyGreaterThan(erosion, 0, 'Erosion rate must be positive');
        end

        function test_neutralizer_power(testCase)
        % Neutralizer power should equal keeper voltage * keeper current.
            P_neut = testCase.thruster.neutralizer_power(20, 0.5);
            testCase.verifyEqual(P_neut, 10, 'AbsTol', 1e-10, ...
                'Neutralizer power should be 20 * 0.5 = 10 W');
        end

        function test_default_propellant_mass(testCase)
        % Constructor with default propellant mass should use xenon value.
            t = GriddedIonThruster(1500, 2.0, 1100, -200, 5e-6);
            testCase.verifyEqual(t.propellant_mass, 2.18e-25, 'AbsTol', 1e-30);
        end

    end
end
