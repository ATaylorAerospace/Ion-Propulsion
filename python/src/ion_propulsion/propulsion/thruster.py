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
    power, efficiencies, Child--Langmuir current density, grid erosion, and
    neutralizer power consumption.  All returned quantities carry astropy
    units for dimensional safety.

    Parameters
    ----------
    beam_voltage_V : float
        Net accelerating (beam / screen) voltage in volts.
    beam_current_A : float
        Ion beam current in amperes.
    screen_grid_voltage_V : float
        Screen grid voltage in volts (positive).
    accel_grid_voltage_V : float
        Accelerator grid voltage magnitude in volts (positive value;
        the grid is biased negative).
    mass_flow_rate_kgs : float
        Propellant mass flow rate in kg/s.
    propellant_mass_kg : float, optional
        Single-ion mass in kg.  Defaults to xenon (2.18e-25 kg).
    grid_spacing_m : float
        Screen-to-accel grid spacing in metres.
    screen_aperture_radius_m : float
        Radius of a single screen grid aperture in metres.
    discharge_loss_WperA : float, optional
        Discharge chamber loss per ampere of beam current in W/A.
        Defaults to 200.0.
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
    def specific_impulse_s(self):
        r"""Specific impulse incorporating extraction efficiency.

        .. math::

            I_{sp} = \\frac{1}{g_0} \\sqrt{\\frac{2\\,e\\,V_{beam}}{m_{ion}}}
                     \\; \\eta_{\\text{extract}}

        where :math:`\\eta_{\\text{extract}} = \\eta_e \\, \\eta_m`.

        Returns
        -------
        Isp : astropy.units.Quantity
            Specific impulse in seconds.
        """
        v_exhaust = np.sqrt(
            2.0 * E_CHARGE * self.beam_voltage_V * u.V / (self.propellant_mass_kg * u.kg)
        )
        eta = self._eta_extraction()
        isp = (v_exhaust * eta / G0).to(u.s)
        return isp

    @property
    def thrust_N(self):
        r"""Thrust produced by the ion beam.

        .. math::

            F = \\dot{m} \\, I_{sp} \\, g_0

        Returns
        -------
        F : astropy.units.Quantity
            Thrust in newtons.
        """
        isp_val = self.specific_impulse_s.value  # in seconds
        g0_val = G0.value  # in m/s^2
        F_val = self.mass_flow_rate_kgs * isp_val * g0_val  # in N
        return F_val * u.N

    @property
    def beam_power_W(self):
        r"""Electrical power deposited into the ion beam.

        .. math::

            P_{beam} = V_{beam} \\, I_{beam}

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

            P_{total} = P_{beam} + I_{beam} \\, \\epsilon_d

        where :math:`\\epsilon_d` is the discharge loss per ampere.

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

            \\eta_e = \\frac{P_{beam}}{P_{total}}

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

            \\eta_m = \\frac{I_{beam} \\, m_{ion}}{e \\, \\dot{m}}

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

        .. math::

            \\eta_{\\text{extract}} = \\eta_e \\, \\eta_m

        Returns
        -------
        eta : float
            Dimensionless combined efficiency.
        """
        return self._eta_extraction()

    @property
    def child_langmuir_current_A(self):
        r"""Child--Langmuir space-charge-limited current density.

        .. math::

            J_{CL} = \\frac{4}{9} \\, \\varepsilon_0
                     \\sqrt{\\frac{2\\,e}{m_{ion}}}
                     \\; \\frac{V_{total}^{3/2}}{d^2}

        where :math:`V_{total} = V_{screen} + V_{accel}` and :math:`d` is
        the grid gap.

        Returns
        -------
        J_CL : astropy.units.Quantity
            Space-charge-limited current density in A/m^2.
        """
        V_total = (self.screen_grid_voltage_V + self.accel_grid_voltage_V) * u.V
        d = self.grid_spacing_m * u.m
        coeff = (4.0 / 9.0) * EPSILON_0 * np.sqrt(
            2.0 * E_CHARGE / (self.propellant_mass_kg * u.kg)
        )
        J_CL = coeff * V_total ** 1.5 / d**2
        return J_CL.to(u.A / u.m**2)

    def grid_erosion_rate_kgs(self, sputter_yield: float = 0.1):
        r"""Estimate the grid mass erosion rate due to ion sputtering.

        .. math::

            \\dot{m}_{\\text{erosion}} = Y_s \\, \\frac{I_{beam} \\, m_{grid}}{e}

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
        erosion = sputter_yield * self.beam_current_A * u.A * M_MOLYBDENUM / E_CHARGE
        return erosion.to(u.kg / u.s)

    def neutralizer_power_W(
        self, keeper_voltage: float = 20.0, keeper_current: float = 2.0
    ):
        r"""Power consumed by the cathode neutralizer.

        .. math::

            P_{neut} = V_{keeper} \\, I_{keeper}

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
        return (keeper_voltage * keeper_current) * u.W

    # ----- Internal helpers ------------------------------------------------

    def _eta_extraction(self) -> float:
        """Combined extraction efficiency (eta_e * eta_m)."""
        return self.electrical_efficiency * self.mass_utilization
