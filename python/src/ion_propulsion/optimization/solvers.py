# Author: A Taylor | Purpose: Mission optimization and Tsiolkovsky-based solvers | Ref: Goebel/Katz/Vallado

import numpy as np
import astropy.units as u


# ---------------------------------------------------------------------------
# Constants (cross-validated across all language implementations)
# ---------------------------------------------------------------------------
G0 = 9.80665  # m/s^2
SECONDS_PER_JULIAN_YEAR = 31557600.0  # 365.25 d

# Specific impulse search window shared by every language implementation.
ISP_SEARCH_MIN_S = 500.0
ISP_SEARCH_MAX_S = 20000.0
BISECTION_MAX_ITER = 200
BISECTION_TOL_S = 1.0e-9


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

    Raises
    ------
    ValueError
        If the specific impulse is not positive.
    """
    if Isp_s <= 0:
        raise ValueError("Specific impulse must be positive")
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

    Raises
    ------
    ValueError
        If the initial mass or specific impulse is not positive.
    """
    if m_initial_kg <= 0:
        raise ValueError("Initial mass must be positive")
    m_prop = m_initial_kg * (1.0 - optimal_payload_fraction(Isp_s, delta_v_ms))
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

    Raises
    ------
    ValueError
        If the mass flow rate is not positive.
    """
    if mass_flow_rate_kgs <= 0:
        raise ValueError("Mass flow rate must be positive")
    t = propellant_mass_kg_val / mass_flow_rate_kgs
    return t * u.s


def power_limited_burn_time_s(
    Isp_s: float,
    delta_v_ms: float,
    power_W: float,
    total_mass_kg: float,
    eta: float = 0.7,
) -> float:
    r"""Burn time needed to deliver a delta-v with a fixed electrical power.

    With jet power :math:`P_{jet} = \eta P = \tfrac{1}{2}\dot{m} v_e^2`
    (Goebel and Katz eq. 2.3-9) the mass flow rate available at a given
    exhaust velocity :math:`v_e = I_{sp} g_0` is

    .. math::

        \dot{m} = \frac{2\,\eta\,P}{v_e^2}, \qquad
        F = \dot{m}\,v_e = \frac{2\,\eta\,P}{v_e}

    and the burn time follows from the Tsiolkovsky propellant mass:

    .. math::

        t_b = \frac{m_{prop}}{\dot{m}}
            = \frac{m_0 \left(1 - e^{-\Delta v / v_e}\right) v_e^2}{2\,\eta\,P}

    Parameters
    ----------
    Isp_s : float
        Specific impulse in seconds.
    delta_v_ms : float
        Mission delta-v in m/s.
    power_W : float
        Electrical power delivered to the thruster in watts.
    total_mass_kg : float
        Initial (wet) spacecraft mass in kg.
    eta : float, optional
        Total thruster efficiency (0 to 1).  Default 0.7.

    Returns
    -------
    t_b : float
        Burn time in seconds.
    """
    if Isp_s <= 0:
        raise ValueError("Specific impulse must be positive")
    if power_W <= 0:
        raise ValueError("Power must be positive")
    if total_mass_kg <= 0:
        raise ValueError("Total mass must be positive")
    if eta <= 0 or eta > 1:
        raise ValueError("Efficiency must be in (0, 1]")

    ve = Isp_s * G0
    m_prop = total_mass_kg * (1.0 - np.exp(-delta_v_ms / ve))
    m_dot = 2.0 * eta * power_W / (ve * ve)
    return float(m_prop / m_dot)


def optimize_isp_for_mission(
    delta_v_ms: float,
    power_W: float,
    total_mass_kg: float,
    eta: float = 0.7,
    max_burn_time_s: float = SECONDS_PER_JULIAN_YEAR,
):
    r"""Find the specific impulse that maximises payload fraction under a
    fixed power budget and a maximum burn time.

    The payload fraction :math:`f = e^{-\Delta v / (I_{sp} g_0)}` increases
    monotonically with :math:`I_{sp}`, but at fixed power a higher
    :math:`I_{sp}` means a lower mass flow rate and a longer burn
    (:func:`power_limited_burn_time_s`), which also increases monotonically.
    The optimum is therefore the largest :math:`I_{sp}` in the search window
    :math:`[500, 20000]` s whose burn time does not exceed the budget:

    .. math::

        I_{sp}^{\ast} = \max \left\{ I_{sp} : t_b(I_{sp}) \le t_{max} \right\}

    It is located by bisection, so every language implementation converges
    to the same value to well within :math:`10^{-6}` relative tolerance.

    Parameters
    ----------
    delta_v_ms : float
        Required mission delta-v in m/s (positive).
    power_W : float
        Available electrical power in watts.
    total_mass_kg : float
        Total spacecraft mass (wet) in kg.
    eta : float, optional
        Total thruster efficiency (0 to 1).  Default 0.7.
    max_burn_time_s : float, optional
        Maximum allowed burn time in seconds.  Default one Julian year.

    Returns
    -------
    optimal_Isp : astropy.units.Quantity
        Optimal specific impulse in seconds.
    max_payload_fraction : float
        Payload fraction achieved at the optimal specific impulse.

    Raises
    ------
    ValueError
        If any input is out of range, or if even the minimum search
        specific impulse cannot deliver the delta-v within the burn-time
        budget.
    """
    if delta_v_ms <= 0:
        raise ValueError("Delta-v must be positive")
    if power_W <= 0:
        raise ValueError("Power must be positive")
    if total_mass_kg <= 0:
        raise ValueError("Total mass must be positive")
    if eta <= 0 or eta > 1:
        raise ValueError("Efficiency must be in (0, 1]")
    if max_burn_time_s <= 0:
        raise ValueError("Maximum burn time must be positive")

    def burn_time(isp: float) -> float:
        return power_limited_burn_time_s(isp, delta_v_ms, power_W, total_mass_kg, eta)

    if burn_time(ISP_SEARCH_MIN_S) > max_burn_time_s:
        raise ValueError(
            "Mission infeasible: the burn time at the minimum search Isp "
            f"({ISP_SEARCH_MIN_S:.0f} s) exceeds the {max_burn_time_s:.4g} s budget"
        )

    if burn_time(ISP_SEARCH_MAX_S) <= max_burn_time_s:
        best_isp = ISP_SEARCH_MAX_S
    else:
        lo, hi = ISP_SEARCH_MIN_S, ISP_SEARCH_MAX_S
        for _ in range(BISECTION_MAX_ITER):
            mid = 0.5 * (lo + hi)
            if burn_time(mid) <= max_burn_time_s:
                lo = mid
            else:
                hi = mid
            if hi - lo < BISECTION_TOL_S:
                break
        best_isp = lo

    return best_isp * u.s, optimal_payload_fraction(best_isp, delta_v_ms)
