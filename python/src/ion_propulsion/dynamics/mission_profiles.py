# Author: A Taylor | Purpose: Orbital mechanics and mission profile calculations | Ref: Goebel/Katz/Vallado

import numpy as np
import astropy.units as u


# ---------------------------------------------------------------------------
# Constants (cross-validated across all language implementations)
# ---------------------------------------------------------------------------
MU_EARTH = 3.986004418e14 * u.m**3 / u.s**2
R_EARTH = 6.371e6 * u.m
GEO_RADIUS = 42164.0e3 * u.m


def geo_transfer_delta_v(r_park_km: float):
    r"""Compute Hohmann transfer delta-v from a circular parking orbit to GEO.

    The two impulsive burns are derived from vis-viva:

    .. math::

        v_{\\text{park}} = \\sqrt{\\frac{\\mu}{r_{\\text{park}}}}

        v_{\\text{geo}} = \\sqrt{\\frac{\\mu}{r_{\\text{geo}}}}

        a_t = \\frac{r_{\\text{park}} + r_{\\text{geo}}}{2}

        v_{t,p} = \\sqrt{\\mu \\left(\\frac{2}{r_{\\text{park}}} - \\frac{1}{a_t}\\right)}

        v_{t,a} = \\sqrt{\\mu \\left(\\frac{2}{r_{\\text{geo}}} - \\frac{1}{a_t}\\right)}

        \\Delta v_1 = v_{t,p} - v_{\\text{park}}, \\quad
        \\Delta v_2 = v_{\\text{geo}} - v_{t,a}

    Parameters
    ----------
    r_park_km : float
        Altitude of the circular parking orbit above Earth's surface in km.

    Returns
    -------
    delta_v1 : astropy.units.Quantity
        First burn delta-v (perigee kick) in km/s.
    delta_v2 : astropy.units.Quantity
        Second burn delta-v (apogee circularization) in km/s.
    """
    r_park = R_EARTH + r_park_km * u.km
    r_geo = GEO_RADIUS

    v_park = np.sqrt(MU_EARTH / r_park)
    v_geo = np.sqrt(MU_EARTH / r_geo)

    a_transfer = (r_park + r_geo) / 2.0

    v_transfer_peri = np.sqrt(MU_EARTH * (2.0 / r_park - 1.0 / a_transfer))
    v_transfer_apo = np.sqrt(MU_EARTH * (2.0 / r_geo - 1.0 / a_transfer))

    delta_v1 = (v_transfer_peri - v_park).to(u.km / u.s)
    delta_v2 = (v_geo - v_transfer_apo).to(u.km / u.s)

    return delta_v1, delta_v2


def lagrange_point_l1(m1_kg: float, m2_kg: float, R_m: float):
    r"""Approximate distance of the L1 Lagrange point from the smaller body.

    .. math::

        r_{L1} = R \\left(\\frac{m_2}{3\\,m_1}\\right)^{1/3}

    Parameters
    ----------
    m1_kg : float
        Mass of the larger body in kg.
    m2_kg : float
        Mass of the smaller body in kg.
    R_m : float
        Distance between the two bodies in metres.

    Returns
    -------
    r_L1 : astropy.units.Quantity
        Distance from the smaller body to L1 in metres.
    """
    R = R_m * u.m
    r_l1 = R * (m2_kg / (3.0 * m1_kg)) ** (1.0 / 3.0)
    return r_l1.to(u.m)


def low_thrust_transfer_time(delta_v_ms: float, thrust_N: float, mass_kg: float):
    r"""Estimate the time required for a constant-thrust low-thrust transfer.

    .. math::

        t = \\frac{m \\, \\Delta v}{F}

    Parameters
    ----------
    delta_v_ms : float
        Required delta-v in m/s.
    thrust_N : float
        Constant thrust in newtons.
    mass_kg : float
        Spacecraft mass in kg.

    Returns
    -------
    t : astropy.units.Quantity
        Transfer time in seconds.
    """
    t = (mass_kg * u.kg * delta_v_ms * u.m / u.s) / (thrust_N * u.N)
    return t.to(u.s)


def spiral_delta_v(r_initial_m: float, r_final_m: float):
    r"""Delta-v for a low-thrust spiral transfer between circular orbits.

    For continuous low-thrust propulsion the delta-v is the difference of the
    circular velocities at the initial and final radii (Edelbaum approximation
    for coplanar transfer):

    .. math::

        \\Delta v = \\sqrt{\\frac{\\mu}{r_i}} - \\sqrt{\\frac{\\mu}{r_f}}

    Parameters
    ----------
    r_initial_m : float
        Initial orbital radius in metres.
    r_final_m : float
        Final orbital radius in metres.

    Returns
    -------
    delta_v : astropy.units.Quantity
        Required delta-v in m/s.
    """
    r_i = r_initial_m * u.m
    r_f = r_final_m * u.m
    delta_v = np.sqrt(MU_EARTH / r_i) - np.sqrt(MU_EARTH / r_f)
    return delta_v.to(u.m / u.s)
