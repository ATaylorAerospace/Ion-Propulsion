# Author: A Taylor | Purpose: Mission optimization and Tsiolkovsky-based solvers | Ref: Goebel/Katz/Vallado

import numpy as np
import astropy.units as u
from scipy.optimize import minimize_scalar


# ---------------------------------------------------------------------------
# Constants (cross-validated across all language implementations)
# ---------------------------------------------------------------------------
G0 = 9.80665  # m/s^2


def optimal_payload_fraction(Isp_s: float, delta_v_ms: float) -> float:
    r"""Payload mass fraction from the Tsiolkovsky rocket equation.

    .. math::

        f_{payload} = \exp\!\left(-\frac{\Delta v}{I_{sp}\,g_0}\right)

    Parameters
    ----------
    Isp_s : float
        Specific impulse in seconds.
    delta_v_ms : float
        Mission delta-v in m/s.

    Returns
    -------
    fraction : float
        Payload mass fraction (dimensionless, 0 to 1).
    """
    return float(np.exp(-delta_v_ms / (Isp_s * G0)))


def propellant_mass_kg(m_initial_kg: float, Isp_s: float, delta_v_ms: float):
    r"""Propellant mass required for a given delta-v via Tsiolkovsky.

    .. math::

        m_{prop} = m_0 \left(1 - \exp\!\left(-\frac{\Delta v}{I_{sp}\,g_0}\right)\right)

    Parameters
    ----------
    m_initial_kg : float
        Initial (wet) spacecraft mass in kg.
    Isp_s : float
        Specific impulse in seconds.
    delta_v_ms : float
        Mission delta-v in m/s.

    Returns
    -------
    m_prop : astropy.units.Quantity
        Required propellant mass in kg.
    """
    m_prop = m_initial_kg * (1.0 - np.exp(-delta_v_ms / (Isp_s * G0)))
    return m_prop * u.kg


def mission_lifetime_s(propellant_mass_kg_val: float, mass_flow_rate_kgs: float):
    r"""Mission lifetime determined by propellant budget.

    .. math::

        t = \frac{m_{prop}}{\dot{m}}

    Parameters
    ----------
    propellant_mass_kg_val : float
        Available propellant mass in kg.
    mass_flow_rate_kgs : float
        Propellant mass flow rate in kg/s.

    Returns
    -------
    t : astropy.units.Quantity
        Mission lifetime in seconds.
    """
    t = propellant_mass_kg_val / mass_flow_rate_kgs
    return t * u.s


def optimize_isp_for_mission(
    delta_v_ms: float,
    power_W: float,
    total_mass_kg: float,
    propellant_mass_kg_ion: float = 2.18e-25,
    eta: float = 0.6,
):
    r"""Find the specific impulse that maximises the payload fraction.

    Given a fixed electrical power budget, thrust is related to Isp by:

    .. math::

        F = \frac{2\,\eta\,P}{I_{sp}\,g_0}

    The mass flow rate and burn time follow, and the objective is to maximise:

    .. math::

        f_{payload} = 1 - \frac{m_{prop}}{m_{total}}

    where the propellant mass is derived from Tsiolkovsky.

    Parameters
    ----------
    delta_v_ms : float
        Required mission delta-v in m/s.
    power_W : float
        Available electrical power in watts.
    total_mass_kg : float
        Total spacecraft mass (wet) in kg.
    propellant_mass_kg_ion : float, optional
        Single-ion mass in kg.  Default xenon.
    eta : float, optional
        Overall thruster efficiency.  Default 0.6.

    Returns
    -------
    optimal_Isp : astropy.units.Quantity
        Optimal specific impulse in seconds.
    max_payload_fraction : float
        Maximised payload fraction.
    """

    def neg_payload_fraction(Isp_s):
        """Negative payload fraction (for minimisation)."""
        if Isp_s <= 0:
            return 0.0
        ve = Isp_s * G0
        pf = np.exp(-delta_v_ms / ve)
        m_prop = total_mass_kg * (1.0 - pf)
        thrust = 2.0 * eta * power_W / ve
        m_dot = thrust / ve
        if m_dot <= 0 or m_prop <= 0:
            return 0.0
        fraction = 1.0 - m_prop / total_mass_kg
        return -fraction

    result = minimize_scalar(
        neg_payload_fraction,
        bounds=(500.0, 20000.0),
        method="bounded",
    )

    optimal_Isp = result.x * u.s
    max_payload_fraction = -result.fun
    return optimal_Isp, max_payload_fraction
