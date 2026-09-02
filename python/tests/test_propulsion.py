# Author: A Taylor | Purpose: Unit tests for gridded ion thruster model | Ref: Goebel/Katz/Vallado

import pytest
import astropy.units as u

from ion_propulsion.propulsion.thruster import GriddedIonThruster, G0


# ---------------------------------------------------------------------------
# Reference operating points
# ---------------------------------------------------------------------------
# NSTAR at full power (Deep Space 1): 1100 V beam, 1.76 A, 3.0 mg/s xenon.
# Flight performance: Isp ~3100 s, thrust ~92 mN, input power ~2.3 kW.
NSTAR = dict(
    beam_voltage_V=1100.0,
    beam_current_A=1.76,
    screen_grid_voltage_V=1100.0,
    accel_grid_voltage_V=-180.0,
    mass_flow_rate_kgs=3.0e-6,
)
NSTAR_FLIGHT_ISP_S = 3100.0
NSTAR_FLIGHT_THRUST_N = 0.092
NSTAR_FLIGHT_POWER_W = 2300.0
BENCHMARK_TOL = 0.10  # beam divergence and doubly charged ions are not modelled

# Golden values shared verbatim with the C++ and MATLAB suites. Every
# language must reproduce them to 1e-6 relative tolerance.
GOLDEN = {
    "exhaust_velocity_ms": 40210.39047562004,
    "mass_utilization": 0.7982474005630351,
    "electrical_efficiency": 0.8461538461538461,
    "ion_extraction_efficiency": 0.675440108168722,
    "specific_impulse_s": 3273.068751590841,
    "thrust_N": 0.09629351901836497,
    "beam_power_W": 1936.0,
    "total_power_W": 2288.0,
    "child_langmuir_Am2": 218.48574875215593,
    "grid_erosion_kgs": 1.7510179217855205e-07,
    "neutralizer_W": 40.0,
}
REL_TOL = 1e-6


def rel_close(actual: float, expected: float, rel: float = REL_TOL) -> bool:
    return abs(actual - expected) <= rel * abs(expected)


@pytest.fixture
def nstar():
    return GriddedIonThruster(**NSTAR)


@pytest.fixture
def high_voltage():
    """1500 V operating point used for the Isp >= 3000 s design requirement."""
    return GriddedIonThruster(
        beam_voltage_V=1500.0,
        beam_current_A=1.8,
        screen_grid_voltage_V=1500.0,
        accel_grid_voltage_V=250.0,
        mass_flow_rate_kgs=3.5e-6,
        grid_spacing_m=0.7e-3,
        screen_aperture_radius_m=0.95e-3,
    )


class TestNstarBenchmark:
    """Model output versus NSTAR flight data at full power."""

    def test_isp_within_10pct_of_flight(self, nstar):
        isp = nstar.specific_impulse_s.value
        assert abs(isp - NSTAR_FLIGHT_ISP_S) / NSTAR_FLIGHT_ISP_S < BENCHMARK_TOL

    def test_thrust_within_10pct_of_flight(self, nstar):
        F = nstar.thrust_N.value
        assert abs(F - NSTAR_FLIGHT_THRUST_N) / NSTAR_FLIGHT_THRUST_N < BENCHMARK_TOL

    def test_input_power_within_10pct_of_flight(self, nstar):
        P = nstar.total_power_W.value
        assert abs(P - NSTAR_FLIGHT_POWER_W) / NSTAR_FLIGHT_POWER_W < BENCHMARK_TOL


class TestGoldenValues:
    """Cross-language parity: identical numbers to 1e-6 in Python, C++, MATLAB."""

    @pytest.mark.parametrize(
        "name, getter",
        [
            ("exhaust_velocity_ms", lambda t: t.exhaust_velocity_ms.value),
            ("mass_utilization", lambda t: t.mass_utilization),
            ("electrical_efficiency", lambda t: t.electrical_efficiency),
            ("ion_extraction_efficiency", lambda t: t.ion_extraction_efficiency),
            ("specific_impulse_s", lambda t: t.specific_impulse_s.value),
            ("thrust_N", lambda t: t.thrust_N.value),
            ("beam_power_W", lambda t: t.beam_power_W.value),
            ("total_power_W", lambda t: t.total_power_W.value),
            ("child_langmuir_Am2", lambda t: t.child_langmuir_current_A.value),
            ("grid_erosion_kgs", lambda t: t.grid_erosion_rate_kgs().value),
            ("neutralizer_W", lambda t: t.neutralizer_power_W().value),
        ],
    )
    def test_golden(self, nstar, name, getter):
        actual = getter(nstar)
        assert rel_close(actual, GOLDEN[name]), f"{name}: {actual!r} != {GOLDEN[name]!r}"


class TestHighEfficiencyRequirement:
    def test_isp_at_least_3000_s(self, high_voltage):
        assert high_voltage.specific_impulse_s.value >= 3000.0

    def test_isp_never_exceeds_ideal(self, high_voltage):
        ideal = high_voltage.exhaust_velocity_ms / G0
        assert high_voltage.specific_impulse_s.value <= ideal.to(u.s).value


class TestThrustAndUnits:
    def test_thrust_equals_eta_m_mdot_vb(self, nstar):
        expected = nstar.mass_utilization * NSTAR["mass_flow_rate_kgs"] * nstar.exhaust_velocity_ms.value
        assert rel_close(nstar.thrust_N.value, expected)

    def test_quantities_carry_expected_units(self, nstar):
        assert nstar.specific_impulse_s.unit == u.s
        assert nstar.thrust_N.unit == u.N
        assert nstar.exhaust_velocity_ms.unit == u.m / u.s
        assert nstar.child_langmuir_current_A.unit == u.A / u.m**2
        assert nstar.grid_erosion_rate_kgs().unit == u.kg / u.s
        assert nstar.total_power_W.unit == u.W


class TestEfficiencies:
    def test_bounded(self, nstar):
        assert 0.0 < nstar.mass_utilization <= 1.0
        assert 0.0 < nstar.electrical_efficiency < 1.0
        assert rel_close(
            nstar.ion_extraction_efficiency,
            nstar.mass_utilization * nstar.electrical_efficiency,
        )


class TestChildLangmuir:
    def test_accel_sign_convention_is_irrelevant(self):
        neg = GriddedIonThruster(**{**NSTAR, "accel_grid_voltage_V": -180.0})
        pos = GriddedIonThruster(**{**NSTAR, "accel_grid_voltage_V": 180.0})
        assert neg.child_langmuir_current_A == pos.child_langmuir_current_A

    def test_non_positive_total_voltage_raises(self):
        t = GriddedIonThruster(**{**NSTAR, "screen_grid_voltage_V": -500.0, "accel_grid_voltage_V": 100.0})
        with pytest.raises(ValueError):
            _ = t.child_langmuir_current_A


class TestValidation:
    @pytest.mark.parametrize(
        "override",
        [
            {"beam_voltage_V": 0.0},
            {"beam_current_A": 0.0},
            {"mass_flow_rate_kgs": 0.0},
            {"grid_spacing_m": 0.0},
            {"propellant_mass_kg": 0.0},
            {"discharge_loss_WperA": -1.0},
        ],
    )
    def test_constructor_rejects_invalid_input(self, override):
        with pytest.raises(ValueError):
            GriddedIonThruster(**{**NSTAR, **override})

    def test_sputter_yield_must_be_positive(self, nstar):
        with pytest.raises(ValueError):
            nstar.grid_erosion_rate_kgs(sputter_yield=0.0)

    def test_keeper_parameters_must_be_positive(self, nstar):
        with pytest.raises(ValueError):
            nstar.neutralizer_power_W(keeper_voltage=0.0)


class TestNeutralizer:
    def test_default_power(self, nstar):
        assert nstar.neutralizer_power_W().value == 40.0

    def test_custom_power(self, nstar):
        assert rel_close(nstar.neutralizer_power_W(15.0, 1.5).value, 22.5)
