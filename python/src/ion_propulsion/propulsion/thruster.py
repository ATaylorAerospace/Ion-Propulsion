# Author: A Taylor | Purpose: Gridded ion thruster performance model | Ref: Goebel/Katz/Vallado

import numpy as np
import astropy.units as u


# ---------------------------------------------------------------------------
# Constants (cross-validated across all language implementations)
# ---------------------------------------------------------------------------
G0 = 9.80665 * u.m / u.s**2
EPSILON_0 = 8.854187817e-12 * u.F / u.m
E_CHARGE = 1.602176634e-19 * u.C
M_XENON = 2.18e-25 * u.kg
M_MOLYBDENUM = 1.594e-25 * u.kg


class GriddedIonThruster:
    r"""Model of a gridded ion thruster (Kaufman-type).

    Provides performance metrics including specific impulse, thrust, beam
    power, efficiencies, Child-Langmuir current density, grid erosion, and
    neutralizer power consumption.  All returned quantities carry astropy
    units for dimensional safety.

    The performance relations follow Goebel and Katz, *Fundamentals of
    Electric Propulsion*, chapter 2.  Beam divergence and multiply charged
    ion corrections are not modelled, so the predicted specific impulse and
    thrust are a few percent above flight data for the same operating point.

    Parameters
    ----------
    beam_voltage_V : float
        Net accelerating (beam / screen) voltage in volts.
    beam_current_A : float
        Ion beam current in amperes.
    screen_grid_voltage_V : float
        Screen grid voltage in volts (positive).
    accel_grid_voltage_V : float
        Accelerator grid voltage in volts.  The grid is biased negative;
        either sign convention is accepted and the magnitude is used.
    mass_flow_rate_kgs : float
        Propellant mass flow rate in kg/s.
    propellant_mass_kg : float, optional
        Single-ion mass in kg.  Defaults to xenon (2.18e-25 kg).
    grid_spacing_m : float, optional
        Screen-to-accel grid spacing in metres.  Default 1 mm.
    screen_aperture_radius_m : float, optional
        Radius of a single screen grid aperture in metres.  Default 1 mm.
    discharge_loss_WperA : float, optional
        Discharge chamber loss per ampere of beam current in W/A
        (numerically equal to eV per beam ion).  Defaults to 200.0.
    """

    def __init__(
        self,
        beam_voltage_V: float,
        beam_current_A: float,
        screen_grid_voltage_V: float,
        accel_grid_voltage_V: float,
        mass_flow_rate_kgs: float,
        propellant_mass_kg: float = 2.18e-25,
        grid_spacing_m: float = 1.0e-3,
        screen_aperture_radius_m: float = 1.0e-3,
        discharge_loss_WperA: float = 200.0,
    ):
        if beam_voltage_V <= 0:
            raise ValueError("Beam voltage must be positive")
        if beam_current_A <= 0:
            raise ValueError("Beam current must be positive")
        if mass_flow_rate_kgs <= 0:
            raise ValueError("Mass flow rate must be positive")
        if grid_spacing_m <= 0:
            raise ValueError("Grid spacing must be positive")
        if propellant_mass_kg <= 0:
            raise ValueError("Propellant mass must be positive")
        if discharge_loss_WperA < 0:
            raise ValueError("Discharge loss must be non-negative")

        self.beam_voltage_V = beam_voltage_V
        self.beam_current_A = beam_current_A
        self.screen_grid_voltage_V = screen_grid_voltage_V
        self.accel_grid_voltage_V = accel_grid_voltage_V
        self.mass_flow_rate_kgs = mass_flow_rate_kgs
        self.propellant_mass_kg = propellant_mass_kg
        self.grid_spacing_m = grid_spacing_m
        self.screen_aperture_radius_m = screen_aperture_radius_m
        self.discharge_loss_WperA = discharge_loss_WperA

    # ----- Performance properties ------------------------------------------

    @property
    def exhaust_velocity_ms(self):
        r"""Ideal ion exhaust velocity for a singly charged ion.

        .. math::

            v_b = \sqrt{\frac{2\,e\,V_{beam}}{m_{ion}}}

        Returns
        -------
        v_b : astropy.units.Quantity
            Exhaust velocity in m/s.
        """
        v_b = np.sqrt(
            2.0 * E_CHARGE * self.beam_voltage_V * u.V / (self.propellant_mass_kg * u.kg)
        )
        return v_b.to(u.m / u.s)

    @property
    def specific_impulse_s(self):
        r"""Effective specific impulse.

        Following Goebel and Katz (eq. 2.4-8, with unit beam divergence and
        no multiply charged ions), only the propellant that is actually
        ionised and accelerated contributes to thrust:

        .. math::

            I_{sp} = \frac{\eta_m}{g_0} \sqrt{\frac{2\,e\,V_{beam}}{m_{ion}}}

        where :math:`\eta_m` is the mass utilization efficiency.  Electrical
        efficiency affects the input power, not the exhaust velocity, so it
        does not appear here.

        Returns
        -------
        Isp : astropy.units.Quantity
            Specific impulse in seconds.
        """
        isp = self.mass_utilization * self.exhaust_velocity_ms / G0
        return isp.to(u.s)

    @property
    def thrust_N(self):
        r"""Thrust produced by the ion beam.

        .. math::

            F = \dot{m} \, I_{sp} \, g_0 = \eta_m \, \dot{m} \, v_b

        Returns
        -------
        F : astropy.units.Quantity
            Thrust in newtons.
        """
        F = self.mass_flow_rate_kgs * u.kg / u.s * self.specific_impulse_s * G0
        return F.to(u.N)

    @property
    def beam_power_W(self):
        r"""Electrical power deposited into the ion beam.

        .. math::

            P_{beam} = V_{beam} \, I_{beam}

        Returns
        -------
        P_beam : astropy.units.Quantity
            Beam power in watts.
        """
        return (self.beam_voltage_V * self.beam_current_A) * u.W

    @property
    def total_power_W(self):
        r"""Total thruster input power including discharge losses.

        .. math::

            P_{total} = P_{beam} + I_{beam} \, \epsilon_d

        where :math:`\epsilon_d` is the discharge loss per ampere.

        Returns
        -------
        P_total : astropy.units.Quantity
            Total power in watts.
        """
        p_beam = self.beam_power_W.value
        p_discharge = self.beam_current_A * self.discharge_loss_WperA
        return (p_beam + p_discharge) * u.W

    @property
    def electrical_efficiency(self):
        r"""Electrical efficiency of the thruster.

        .. math::

            \eta_e = \frac{P_{beam}}{P_{total}}

        Returns
        -------
        eta_e : float
            Dimensionless electrical efficiency.
        """
        return float(self.beam_power_W / self.total_power_W)

    @property
    def mass_utilization(self):
        r"""Propellant mass utilization efficiency.

        .. math::

            \eta_m = \frac{I_{beam} \, m_{ion}}{e \, \dot{m}}

        Returns
        -------
        eta_m : float
            Dimensionless mass utilization efficiency.
        """
        numerator = self.beam_current_A * u.A * self.propellant_mass_kg * u.kg
        denominator = E_CHARGE * self.mass_flow_rate_kgs * u.kg / u.s
        eta_m = (numerator / denominator).decompose()
        return float(eta_m)

    @property
    def ion_extraction_efficiency(self):
        r"""Combined ion extraction efficiency.

        Product of the electrical and mass utilization efficiencies; with
        unit beam divergence this equals the total thruster efficiency
        :math:`\eta_T = \eta_e \, \eta_m` of Goebel and Katz eq. 2.5-7.

        .. math::

            \eta_{\text{extract}} = \eta_e \, \eta_m

        Returns
        -------
        eta : float
            Dimensionless combined efficiency.
        """
        return self.electrical_efficiency * self.mass_utilization

    @property
    def child_langmuir_current_A(self):
        r"""Child-Langmuir space-charge-limited current density.

        .. math::

            J_{CL} = \frac{4}{9} \, \varepsilon_0
                     \sqrt{\frac{2\,e}{m_{ion}}}
                     \; \frac{V_{total}^{3/2}}{d^2}

        where :math:`V_{total} = V_{screen} + |V_{accel}|` is the total
        voltage across the grid gap :math:`d`.  Despite the ``_A`` suffix,
        kept for API compatibility, the value is a current *density*.

        Returns
        -------
        J_CL : astropy.units.Quantity
            Space-charge-limited current density in A/m^2.

        Raises
        ------
        ValueError
            If the total extraction voltage is not positive.
        """
        V_total_val = self.screen_grid_voltage_V + abs(self.accel_grid_voltage_V)
        if V_total_val <= 0:
            raise ValueError("Total extraction voltage (screen + |accel|) must be positive")
        V_total = V_total_val * u.V
        d = self.grid_spacing_m * u.m
        coeff = (4.0 / 9.0) * EPSILON_0 * np.sqrt(
            2.0 * E_CHARGE / (self.propellant_mass_kg * u.kg)
        )
        J_CL = coeff * V_total ** 1.5 / d**2
        return J_CL.to(u.A / u.m**2)

    def grid_erosion_rate_kgs(self, sputter_yield: float = 0.1):
        r"""Estimate the grid mass erosion rate due to ion sputtering.

        .. math::

            \dot{m}_{\text{erosion}} = Y_s \, \frac{I_{beam} \, m_{grid}}{e}

        where :math:`Y_s` is the sputter yield (atoms per ion) and
        :math:`m_{grid}` is the mass of one grid atom (molybdenum).

        Parameters
        ----------
        sputter_yield : float, optional
            Sputter yield in atoms per incident ion.  Default 0.1.

        Returns
        -------
        erosion_rate : astropy.units.Quantity
            Mass erosion rate in kg/s.
        """
        if sputter_yield <= 0:
            raise ValueError("Sputter yield must be positive")
        erosion = sputter_yield * self.beam_current_A * u.A * M_MOLYBDENUM / E_CHARGE
        return erosion.to(u.kg / u.s)

    def neutralizer_power_W(
        self, keeper_voltage: float = 20.0, keeper_current: float = 2.0
    ):
        r"""Power consumed by the cathode neutralizer.

        .. math::

            P_{neut} = V_{keeper} \, I_{keeper}

        Parameters
        ----------
        keeper_voltage : float, optional
            Keeper electrode voltage in volts.  Default 20.0.
        keeper_current : float, optional
            Keeper current in amperes.  Default 2.0.

        Returns
        -------
        P_neut : astropy.units.Quantity
            Neutralizer power in watts.
        """
        if keeper_voltage <= 0 or keeper_current <= 0:
            raise ValueError("Keeper voltage and current must be positive")
        return (keeper_voltage * keeper_current) * u.W
