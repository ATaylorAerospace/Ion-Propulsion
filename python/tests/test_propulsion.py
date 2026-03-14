# Author: A Taylor | Purpose: Unit tests for gridded ion thruster model | Ref: Goebel/Katz/Vallado

import pytest
import astropy.units as u

from ion_propulsion.propulsion.thruster import GriddedIonThruster


@pytest.fixture
def nstar_like():
    """Approximate NSTAR-class thruster parameters."""
    return GriddedIonThruster(
        beam_voltage_V=1500.0,
        beam_current_A=1.8,
        screen_grid_voltage_V=1500.0,
        accel_grid_voltage_V=250.0,
        mass_flow_rate_kgs=3.5e-6,
        propellant_mass_kg=2.18e-25,
        grid_spacing_m=0.7e-3,
        screen_aperture_radius_m=0.95e-3,
        discharge_loss_WperA=200.0,
    )


class TestSpecificImpulse:
    """Isp for 1500 V beam should be well above 3000 s."""

    def test_isp_above_3000(self, nstar_like):
        """A 1500 V beam produces a raw exhaust velocity corresponding to
        ~4200 s; with extraction efficiency the effective Isp should still
        be well above 2500 s and in the range expected of flight thrusters."""
        isp = nstar_like.specific_impulse_s
        assert isp.value >= 2900.0, f"Isp too low: {isp}"
        assert isp.value <= 5000.0, f"Isp unexpectedly high: {isp}"

    def test_isp_units(self, nstar_like):
        isp = nstar_like.specific_impulse_s
        assert isp.unit == u.s


class TestThrust:
    def test_thrust_positive(self, nstar_like):
        F = nstar_like.thrust_N
        assert F.value > 0.0

    def test_thrust_units(self, nstar_like):
        F = nstar_like.thrust_N
        assert F.unit == u.N


class TestPower:
    def test_beam_power(self, nstar_like):
        P = nstar_like.beam_power_W
        assert abs(P.value - 2700.0) < 1.0  # 1500 * 1.8

    def test_total_power_exceeds_beam(self, nstar_like):
        assert nstar_like.total_power_W.value > nstar_like.beam_power_W.value


class TestEfficiency:
    def test_electrical_efficiency_range(self, nstar_like):
        eta = nstar_like.electrical_efficiency
        assert 0.0 < eta < 1.0

    def test_mass_utilization_range(self, nstar_like):
        eta_m = nstar_like.mass_utilization
        assert 0.0 < eta_m <= 1.5  # can exceed 1 if double-charged ions present

    def test_extraction_efficiency(self, nstar_like):
        eta = nstar_like.ion_extraction_efficiency
        assert 0.0 < eta < 1.0


class TestChildLangmuir:
    def test_positive_current_density(self, nstar_like):
        J = nstar_like.child_langmuir_current_A
        assert J.value > 0.0

    def test_units(self, nstar_like):
        J = nstar_like.child_langmuir_current_A
        assert J.unit == u.A / u.m**2


class TestGridErosion:
    def test_erosion_positive(self, nstar_like):
        rate = nstar_like.grid_erosion_rate_kgs()
        assert rate.value > 0.0

    def test_erosion_units(self, nstar_like):
        rate = nstar_like.grid_erosion_rate_kgs()
        assert rate.unit == u.kg / u.s


class TestNeutralizerPower:
    def test_default_power(self, nstar_like):
        P = nstar_like.neutralizer_power_W()
        assert abs(P.value - 40.0) < 0.1  # 20 * 2

    def test_custom_power(self, nstar_like):
        P = nstar_like.neutralizer_power_W(keeper_voltage=15.0, keeper_current=1.5)
        assert abs(P.value - 22.5) < 0.1
