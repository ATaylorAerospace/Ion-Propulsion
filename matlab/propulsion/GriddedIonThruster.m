% Author: A Taylor | Purpose: Gridded ion thruster performance model | Ref: Goebel/Katz/Vallado
classdef GriddedIonThruster
% GRIDDEDIONTHRUSTER Performance model for a gridded ion propulsion system.
%
%   Encapsulates the key operating parameters of a gridded ion thruster
%   and provides methods for computing performance metrics including
%   specific impulse, thrust, efficiencies, Child-Langmuir current
%   density, and grid erosion rate.
%
%   Example:
%       t = GriddedIonThruster(1500, 2.0, 1100, -200, 5e-6, ...
%               2.18e-25, 1e-3, 1e-3, 200);
%       fprintf('Isp = %.1f s\n', t.specific_impulse());

    properties
        beam_voltage          double  % Screen/beam voltage (V)
        beam_current          double  % Ion beam current (A)
        screen_grid_voltage   double  % Screen grid voltage (V)
        accel_grid_voltage    double  % Accelerator grid voltage (V)
        mass_flow_rate        double  % Propellant mass flow rate (kg/s)
        propellant_mass       double  % Single-ion mass (kg)
        grid_spacing          double  % Screen-to-accel grid spacing (m)
        screen_aperture_radius double % Screen grid aperture radius (m)
        discharge_loss        double  % Discharge loss per beam ion (W/A)
    end

    properties (Constant, Access = private)
        g0        = 9.80665;            % Standard gravitational acceleration (m/s^2)
        epsilon_0 = 8.854187817e-12;    % Vacuum permittivity (F/m)
        e_charge  = 1.602176634e-19;    % Elementary charge (C)
        m_xenon   = 2.18e-25;           % Xenon ion mass (kg)
    end

    methods
        function obj = GriddedIonThruster(beam_voltage, beam_current, ...
                screen_grid_voltage, accel_grid_voltage, mass_flow_rate, ...
                propellant_mass, grid_spacing, screen_aperture_radius, ...
                discharge_loss)
        % GriddedIonThruster Construct a gridded ion thruster model.
        %
        %   obj = GriddedIonThruster(beam_voltage, beam_current,
        %       screen_grid_voltage, accel_grid_voltage, mass_flow_rate,
        %       propellant_mass, grid_spacing, screen_aperture_radius,
        %       discharge_loss)
        %
        %   Parameters:
        %       beam_voltage          - Beam/screen voltage (V)
        %       beam_current          - Beam current (A)
        %       screen_grid_voltage   - Screen grid voltage (V)
        %       accel_grid_voltage    - Accelerator grid voltage (V)
        %       mass_flow_rate        - Propellant mass flow rate (kg/s)
        %       propellant_mass       - Propellant ion mass (kg), default 2.18e-25
        %       grid_spacing          - Grid spacing (m)
        %       screen_aperture_radius - Screen aperture radius (m)
        %       discharge_loss        - Discharge loss (W/A), default 200.0

            arguments
                beam_voltage          (1,1) double {mustBePositive}
                beam_current          (1,1) double {mustBePositive}
                screen_grid_voltage   (1,1) double
                accel_grid_voltage    (1,1) double
                mass_flow_rate        (1,1) double {mustBePositive}
                propellant_mass       (1,1) double {mustBePositive} = 2.18e-25
                grid_spacing          (1,1) double {mustBePositive} = 1e-3
                screen_aperture_radius(1,1) double {mustBePositive} = 1e-3
                discharge_loss        (1,1) double {mustBeNonnegative} = 200.0
            end

            obj.beam_voltage           = beam_voltage;
            obj.beam_current           = beam_current;
            obj.screen_grid_voltage    = screen_grid_voltage;
            obj.accel_grid_voltage     = accel_grid_voltage;
            obj.mass_flow_rate         = mass_flow_rate;
            obj.propellant_mass        = propellant_mass;
            obj.grid_spacing           = grid_spacing;
            obj.screen_aperture_radius = screen_aperture_radius;
            obj.discharge_loss         = discharge_loss;
        end

        function Isp = specific_impulse(obj)
        % SPECIFIC_IMPULSE Effective specific impulse of the thruster.
        %
        %   $$I_{sp} = \frac{1}{g_0}\sqrt{\frac{2\,e\,V_{beam}}{m_{ion}}} \cdot \eta_{extraction}$$
        %
        %   where $\eta_{extraction} = \eta_m \cdot \eta_e$.

            eta_ext = obj.mass_utilization() * obj.electrical_efficiency();
            v_exhaust = sqrt(2.0 * obj.e_charge * obj.beam_voltage / obj.propellant_mass);
            Isp = (v_exhaust / obj.g0) * eta_ext;
        end

        function F = thrust(obj)
        % THRUST Effective thrust produced by the thruster (N).
        %
        %   $$F = \dot{m} \cdot I_{sp} \cdot g_0$$

            F = obj.mass_flow_rate * obj.specific_impulse() * obj.g0;
        end

        function P = beam_power(obj)
        % BEAM_POWER Electrical power in the ion beam (W).
        %
        %   $$P_{beam} = V_{beam} \cdot I_{beam}$$

            P = obj.beam_voltage * obj.beam_current;
        end

        function P = total_power(obj)
        % TOTAL_POWER Total input power including discharge losses (W).
        %
        %   $$P_{total} = P_{beam} + I_{beam} \cdot \epsilon_d$$

            P = obj.beam_power() + obj.beam_current * obj.discharge_loss;
        end

        function eta = electrical_efficiency(obj)
        % ELECTRICAL_EFFICIENCY Ratio of beam power to total input power.
        %
        %   $$\eta_e = \frac{P_{beam}}{P_{total}}$$

            eta = obj.beam_power() / obj.total_power();
        end

        function eta = mass_utilization(obj)
        % MASS_UTILIZATION Fraction of propellant mass converted to beam ions.
        %
        %   $$\eta_m = \frac{I_{beam} \cdot m_{ion}}{e \cdot \dot{m}}$$

            eta = (obj.beam_current * obj.propellant_mass) / ...
                  (obj.e_charge * obj.mass_flow_rate);
        end

        function eta = ion_extraction_efficiency(obj)
        % ION_EXTRACTION_EFFICIENCY Combined electrical and mass utilization efficiency.
        %
        %   $$\eta_{ext} = \eta_e \cdot \eta_m$$

            eta = obj.electrical_efficiency() * obj.mass_utilization();
        end

        function J_CL = child_langmuir_current(obj)
        % CHILD_LANGMUIR_CURRENT Space-charge-limited current density (A/m^2).
        %
        %   $$J_{CL} = \frac{4}{9}\,\varepsilon_0\,\sqrt{\frac{2\,e}{m_{ion}}}
        %              \,\frac{V_{total}^{3/2}}{d^2}$$
        %
        %   where $V_{total} = V_{beam} - V_{accel}$ (note: V_accel is
        %   typically negative, so V_total > V_beam).

            V_total = obj.beam_voltage - obj.accel_grid_voltage;
            J_CL = (4.0 / 9.0) * obj.epsilon_0 * ...
                    sqrt(2.0 * obj.e_charge / obj.propellant_mass) * ...
                    V_total^(3.0/2.0) / obj.grid_spacing^2;
        end

        function erosion = grid_erosion_rate(obj, sputter_yield)
        % GRID_EROSION_RATE Estimate accelerator grid erosion rate (kg/s).
        %
        %   $$\dot{m}_{erosion} = Y_s \cdot \frac{I_{beam} \cdot m_{Mo}}{e}$$
        %
        %   Uses the mass of molybdenum ($m_{Mo} = 1.594 \times 10^{-25}$ kg)
        %   as the grid material.
        %
        %   Inputs:
        %       sputter_yield - Sputter yield (atoms/ion)

            arguments
                obj
                sputter_yield (1,1) double {mustBePositive}
            end

            m_molybdenum = 1.594e-25;  % kg per Mo atom
            erosion = sputter_yield * obj.beam_current * m_molybdenum / obj.e_charge;
        end

        function P = neutralizer_power(obj, keeper_voltage, keeper_current)
        % NEUTRALIZER_POWER Power consumed by the neutralizer keeper (W).
        %
        %   $$P_{neut} = V_{keeper} \cdot I_{keeper}$$

            arguments
                obj
                keeper_voltage (1,1) double {mustBePositive}
                keeper_current (1,1) double {mustBePositive}
            end

            P = keeper_voltage * keeper_current;
        end
    end
end
