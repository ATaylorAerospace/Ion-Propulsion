# Author: A Taylor | Purpose: Unit tests for optimization and Tsiolkovsky solvers | Ref: Goebel/Katz/Vallado

import pytest
import astropy.units as u

from ion_propulsion.optimization.solvers import (
    ISP_SEARCH_MAX_S,
    SECONDS_PER_JULIAN_YEAR,
    mission_lifetime_s,
    optimal_payload_fraction,
    optimize_isp_for_mission,
    power_limited_burn_time_s,
    propellant_mass_kg,
)

# Golden values shared verbatim with the C++ and MATLAB suites (1e-6 relative).
PF_3000S_4000MS = 0.8728756599684215
MPROP_1000KG_3000S_4000MS = 127.12434003157847
LIFETIME_100KG_3P5MGS_S = 28571428.57142857
BURN_3000S_5000MS_5KW_1000KG_S = 19325541.892783504
OPTIMUM = {
    # power_W: (optimal Isp s, payload fraction) for dv = 5000 m/s, m0 = 1000 kg,
    # eta = 0.7, one Julian year burn budget
    2000.0: (2036.9951343927255, 0.7785678565566541),
    5000.0: (4751.221131411683, 0.8982463098059607),
    10000.0: (9260.657238489515, 0.946431802719756),
}
REL_TOL = 1e-6

DV = 5000.0
M0 = 1000.0
ETA = 0.7


def rel_close(actual: float, expected: float, rel: float = REL_TOL) -> bool:
    return abs(actual - expected) <= rel * abs(expected)


class TestOptimalPayloadFraction:
    def test_golden(self):
        assert rel_close(optimal_payload_fraction(3000.0, 4000.0), PF_3000S_4000MS)

    def test_zero_delta_v_gives_unity(self):
        assert optimal_payload_fraction(3000.0, 0.0) == 1.0

    def test_monotonic_in_isp_and_delta_v(self):
        assert optimal_payload_fraction(4000.0, DV) > optimal_payload_fraction(2000.0, DV)
        assert optimal_payload_fraction(3000.0, 2000.0) > optimal_payload_fraction(3000.0, 8000.0)

    def test_non_positive_isp_raises(self):
        with pytest.raises(ValueError):
            optimal_payload_fraction(0.0, DV)


class TestPropellantMass:
    def test_golden(self):
        assert rel_close(propellant_mass_kg(1000.0, 3000.0, 4000.0).value, MPROP_1000KG_3000S_4000MS)

    def test_units(self):
        assert propellant_mass_kg(1000.0, 3000.0, 4000.0).unit == u.kg

    def test_zero_delta_v_needs_no_propellant(self):
        assert propellant_mass_kg(1000.0, 3000.0, 0.0).value == 0.0

    def test_non_positive_mass_raises(self):
        with pytest.raises(ValueError):
            propellant_mass_kg(0.0, 3000.0, 4000.0)


class TestMissionLifetime:
    def test_golden(self):
        assert rel_close(mission_lifetime_s(100.0, 3.5e-6).value, LIFETIME_100KG_3P5MGS_S)

    def test_units(self):
        assert mission_lifetime_s(100.0, 3.5e-6).unit == u.s

    def test_non_positive_flow_rate_raises(self):
        with pytest.raises(ValueError):
            mission_lifetime_s(100.0, 0.0)


class TestPowerLimitedBurnTime:
    def test_golden(self):
        t_b = power_limited_burn_time_s(3000.0, DV, 5000.0, M0, ETA)
        assert rel_close(t_b, BURN_3000S_5000MS_5KW_1000KG_S)

    def test_increases_with_isp(self):
        assert power_limited_burn_time_s(4000.0, DV, 5000.0, M0, ETA) > power_limited_burn_time_s(
            3000.0, DV, 5000.0, M0, ETA
        )

    def test_inversely_proportional_to_power(self):
        t_5kw = power_limited_burn_time_s(3000.0, DV, 5000.0, M0, ETA)
        t_10kw = power_limited_burn_time_s(3000.0, DV, 10000.0, M0, ETA)
        assert rel_close(t_5kw / t_10kw, 2.0)

    def test_invalid_efficiency_raises(self):
        with pytest.raises(ValueError):
            power_limited_burn_time_s(3000.0, DV, 5000.0, M0, 1.5)


class TestOptimizeIspForMission:
    @pytest.mark.parametrize("power_W", sorted(OPTIMUM))
    def test_golden(self, power_W):
        isp, pf = optimize_isp_for_mission(DV, power_W, M0, ETA)
        exp_isp, exp_pf = OPTIMUM[power_W]
        assert rel_close(isp.value, exp_isp)
        assert rel_close(pf, exp_pf)

    def test_burn_time_at_optimum_meets_budget(self):
        isp, _ = optimize_isp_for_mission(DV, 5000.0, M0, ETA)
        t_b = power_limited_burn_time_s(isp.value, DV, 5000.0, M0, ETA)
        assert rel_close(t_b, SECONDS_PER_JULIAN_YEAR)

    def test_more_power_allows_higher_isp(self):
        isp_2kw, pf_2kw = optimize_isp_for_mission(DV, 2000.0, M0, ETA)
        isp_10kw, pf_10kw = optimize_isp_for_mission(DV, 10000.0, M0, ETA)
        assert isp_10kw.value > isp_2kw.value
        assert pf_10kw > pf_2kw

    def test_unconstrained_returns_search_upper_bound(self):
        isp, _ = optimize_isp_for_mission(DV, 1.0e6, M0, ETA)
        assert isp.value == ISP_SEARCH_MAX_S

    def test_infeasible_budget_raises(self):
        with pytest.raises(ValueError, match="infeasible"):
            optimize_isp_for_mission(DV, 5000.0, M0, ETA, max_burn_time_s=1.0)

    @pytest.mark.parametrize(
        "kwargs",
        [
            {"delta_v_ms": 0.0},
            {"power_W": 0.0},
            {"total_mass_kg": 0.0},
            {"eta": 0.0},
            {"eta": 1.5},
            {"max_burn_time_s": 0.0},
        ],
    )
    def test_invalid_inputs_raise(self, kwargs):
        args = {"delta_v_ms": DV, "power_W": 5000.0, "total_mass_kg": M0, "eta": ETA}
        args.update(kwargs)
        with pytest.raises(ValueError):
            optimize_isp_for_mission(**args)

    def test_isp_carries_units(self):
        isp, _ = optimize_isp_for_mission(DV, 5000.0, M0, ETA)
        assert isp.unit == u.s
