# Author: A Taylor | Purpose: Unit tests for optimization and Tsiolkovsky solvers | Ref: Goebel/Katz/Vallado

import pytest
import numpy as np
import astropy.units as u

from ion_propulsion.optimization.solvers import (
    mission_lifetime_s,
    optimal_payload_fraction,
    optimize_isp_for_mission,
    propellant_mass_kg,
)


G0 = 9.80665


class TestOptimalPayloadFraction:
    """Tsiolkovsky payload fraction verification."""

    def test_known_value(self):
        """Isp=3000 s, delta_v=4000 m/s should give ~0.872."""
        frac = optimal_payload_fraction(3000.0, 4000.0)
        assert abs(frac - 0.872) < 0.01, f"Payload fraction off: {frac}"

    def test_zero_deltav(self):
        frac = optimal_payload_fraction(3000.0, 0.0)
        assert abs(frac - 1.0) < 1e-10

    def test_high_deltav_small_fraction(self):
        frac = optimal_payload_fraction(300.0, 10000.0)
        assert frac < 0.05

    def test_returns_float(self):
        frac = optimal_payload_fraction(3000.0, 4000.0)
        assert isinstance(frac, float)


class TestPropellantMass:
    def test_basic(self):
        m_prop = propellant_mass_kg(1000.0, 3000.0, 4000.0)
        expected = 1000.0 * (1.0 - np.exp(-4000.0 / (3000.0 * G0)))
        assert abs(m_prop.value - expected) < 0.01

    def test_units(self):
        m_prop = propellant_mass_kg(1000.0, 3000.0, 4000.0)
        assert m_prop.unit == u.kg

    def test_zero_deltav_no_propellant(self):
        m_prop = propellant_mass_kg(1000.0, 3000.0, 0.0)
        assert abs(m_prop.value) < 1e-10


class TestMissionLifetime:
    def test_basic(self):
        t = mission_lifetime_s(100.0, 1e-5)
        assert abs(t.value - 1e7) < 1.0

    def test_units(self):
        t = mission_lifetime_s(100.0, 1e-5)
        assert t.unit == u.s


class TestOptimizeIspForMission:
    def test_returns_valid_isp(self):
        isp, frac = optimize_isp_for_mission(
            delta_v_ms=5000.0,
            power_W=5000.0,
            total_mass_kg=1000.0,
        )
        assert isp.value > 500.0
        assert isp.unit == u.s

    def test_payload_fraction_positive(self):
        _, frac = optimize_isp_for_mission(
            delta_v_ms=5000.0,
            power_W=5000.0,
            total_mass_kg=1000.0,
        )
        assert 0.0 < frac < 1.0

    def test_higher_power_improves_fraction(self):
        """With more power available the optimizer should find a higher
        (or equal) payload fraction since the power constraint is less
        binding."""
        _, frac_low = optimize_isp_for_mission(
            delta_v_ms=5000.0,
            power_W=2000.0,
            total_mass_kg=1000.0,
        )
        _, frac_high = optimize_isp_for_mission(
            delta_v_ms=5000.0,
            power_W=10000.0,
            total_mass_kg=1000.0,
        )
        # Both use Tsiolkovsky only, so fraction should be identical here
        # (power doesn't enter the fraction formula directly in this solver)
        assert frac_high >= frac_low - 1e-6
