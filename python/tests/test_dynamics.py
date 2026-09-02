# Author: A Taylor | Purpose: Unit tests for orbital dynamics module | Ref: Goebel/Katz/Vallado

import pytest
import astropy.units as u

from ion_propulsion.dynamics.mission_profiles import (
    geo_transfer_delta_v,
    lagrange_point_l1,
    low_thrust_transfer_time,
    spiral_delta_v,
)

# Golden values shared verbatim with the C++ and MATLAB suites (1e-6 relative).
GEO_200KM_DV1_KMS = 2.456667863273449
GEO_200KM_DV2_KMS = 1.478021775095608
L1_SUN_EARTH_M = 1496417737.093085
LOW_THRUST_T_S = 25000000.0
SPIRAL_LEO200_GEO_MS = 4713.821700845472
REL_TOL = 1e-6

M_SUN = 1.989e30
M_EARTH = 5.972e24
AU_M = 1.496e11
R_LEO_200KM_M = 6.571e6
R_GEO_M = 42164.0e3


def rel_close(actual: float, expected: float, rel: float = REL_TOL) -> bool:
    return abs(actual - expected) <= rel * abs(expected)


class TestGeoTransferDeltaV:
    def test_golden_first_burn(self):
        dv1, _ = geo_transfer_delta_v(200.0)
        assert rel_close(dv1.value, GEO_200KM_DV1_KMS)

    def test_golden_second_burn(self):
        _, dv2 = geo_transfer_delta_v(200.0)
        assert rel_close(dv2.value, GEO_200KM_DV2_KMS)

    def test_units_are_km_per_s(self):
        dv1, dv2 = geo_transfer_delta_v(200.0)
        assert dv1.unit == u.km / u.s
        assert dv2.unit == u.km / u.s

    def test_higher_parking_orbit_needs_less_delta_v(self):
        low = sum(q.value for q in geo_transfer_delta_v(200.0))
        high = sum(q.value for q in geo_transfer_delta_v(1000.0))
        assert high < low

    def test_non_positive_altitude_raises(self):
        with pytest.raises(ValueError):
            geo_transfer_delta_v(-100.0)


class TestLagrangePointL1:
    def test_golden_sun_earth(self):
        r_l1 = lagrange_point_l1(M_SUN, M_EARTH, AU_M)
        assert rel_close(r_l1.value, L1_SUN_EARTH_M)

    def test_units_are_metres(self):
        assert lagrange_point_l1(M_SUN, M_EARTH, AU_M).unit == u.m

    @pytest.mark.parametrize("args", [(-1.0, 1.0, 1.0), (1.0, -1.0, 1.0), (1.0, 1.0, 0.0)])
    def test_invalid_inputs_raise(self, args):
        with pytest.raises(ValueError):
            lagrange_point_l1(*args)


class TestLowThrustTransferTime:
    def test_golden(self):
        t = low_thrust_transfer_time(delta_v_ms=5000.0, thrust_N=0.1, mass_kg=500.0)
        assert rel_close(t.value, LOW_THRUST_T_S)

    def test_units_are_seconds(self):
        assert low_thrust_transfer_time(1000.0, 0.05, 200.0).unit == u.s

    @pytest.mark.parametrize("thrust, mass", [(0.0, 500.0), (0.1, 0.0)])
    def test_invalid_inputs_raise(self, thrust, mass):
        with pytest.raises(ValueError):
            low_thrust_transfer_time(5000.0, thrust, mass)


class TestSpiralDeltaV:
    def test_golden_leo_to_geo(self):
        dv = spiral_delta_v(R_LEO_200KM_M, R_GEO_M)
        assert rel_close(dv.value, SPIRAL_LEO200_GEO_MS)

    def test_units_are_m_per_s(self):
        assert spiral_delta_v(7.0e6, R_GEO_M).unit == u.m / u.s

    def test_same_radius_is_zero(self):
        assert spiral_delta_v(R_GEO_M, R_GEO_M).value == 0.0

    def test_invalid_radii_raise(self):
        with pytest.raises(ValueError):
            spiral_delta_v(0.0, R_GEO_M)
