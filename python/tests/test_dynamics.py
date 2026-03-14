# Author: A Taylor | Purpose: Unit tests for orbital dynamics module | Ref: Goebel/Katz/Vallado

import pytest
import astropy.units as u

from ion_propulsion.dynamics.mission_profiles import (
    geo_transfer_delta_v,
    lagrange_point_l1,
    low_thrust_transfer_time,
    spiral_delta_v,
)


class TestGeoTransferDeltaV:
    """Verify Hohmann transfer delta-v from LEO parking orbit to GEO."""

    def test_200km_parking_orbit_total_deltav(self):
        """From a 200 km parking orbit the total Hohmann delta-v to GEO
        should be approximately 2.46 + 1.48 = ~3.94 km/s (combined burns)."""
        dv1, dv2 = geo_transfer_delta_v(200.0)
        total = dv1.value + dv2.value
        assert 2.3 < dv1.value < 2.6, f"dv1 out of range: {dv1}"
        assert 1.3 < dv2.value < 1.7, f"dv2 out of range: {dv2}"
        assert 3.7 < total < 4.2, f"total delta-v out of range: {total}"

    def test_units_are_km_per_s(self):
        dv1, dv2 = geo_transfer_delta_v(200.0)
        assert dv1.unit == u.km / u.s
        assert dv2.unit == u.km / u.s

    def test_higher_orbit_lower_deltav(self):
        """A higher parking orbit should require less total delta-v."""
        dv1_low, dv2_low = geo_transfer_delta_v(200.0)
        dv1_high, dv2_high = geo_transfer_delta_v(1000.0)
        total_low = dv1_low.value + dv2_low.value
        total_high = dv1_high.value + dv2_high.value
        assert total_high < total_low


class TestLagrangePointL1:
    """Verify the L1 approximation for the Sun--Earth system."""

    def test_sun_earth_l1(self):
        m_sun = 1.989e30
        m_earth = 5.972e24
        R_se = 1.496e11  # metres
        r_l1 = lagrange_point_l1(m_sun, m_earth, R_se)
        # L1 is roughly 1.5 million km from Earth
        assert 1.4e9 < r_l1.value < 1.6e9

    def test_units_are_metres(self):
        r_l1 = lagrange_point_l1(1.989e30, 5.972e24, 1.496e11)
        assert r_l1.unit == u.m


class TestLowThrustTransferTime:
    """Basic sanity checks on the low-thrust transfer time formula."""

    def test_basic_transfer(self):
        t = low_thrust_transfer_time(
            delta_v_ms=5000.0, thrust_N=0.1, mass_kg=500.0
        )
        expected_s = 500.0 * 5000.0 / 0.1
        assert abs(t.value - expected_s) < 1.0

    def test_units_are_seconds(self):
        t = low_thrust_transfer_time(1000.0, 0.05, 200.0)
        assert t.unit == u.s


class TestSpiralDeltaV:
    """Verify the low-thrust spiral delta-v approximation."""

    def test_leo_to_geo_spiral(self):
        r_leo = 6.571e6  # 200 km altitude
        r_geo = 42164.0e3
        dv = spiral_delta_v(r_leo, r_geo)
        # Spiral delta-v from LEO to GEO is typically ~4.7 km/s
        assert 4.0e3 < dv.value < 5.5e3

    def test_units_are_m_per_s(self):
        dv = spiral_delta_v(7.0e6, 42164.0e3)
        assert dv.unit == u.m / u.s
