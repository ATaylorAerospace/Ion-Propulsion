![Geosat](docs/geosats.png)

# 🚀 **Ion-Propulsion: Gridded Ion Thruster Suite**

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Stars](https://img.shields.io/github/stars/ATaylorAerospace/Ion-Propulsion?style=social)](https://github.com/ATaylorAerospace/Ion-Propulsion)
[![Languages](https://img.shields.io/badge/Languages-Python%20%7C%20C%2B%2B%20%7C%20MATLAB-brightgreen.svg)](https://github.com/ATaylorAerospace/Ion-Propulsion)
[![Python](https://img.shields.io/badge/Python-3.10%2B-blue.svg)](https://www.python.org/)
[![C++](https://img.shields.io/badge/C%2B%2B-20-orange.svg)](https://isocpp.org/)
[![MATLAB](https://img.shields.io/badge/MATLAB-R2020b%2B-orange.svg)](https://www.mathworks.com/products/matlab.html)
[![Tests](https://img.shields.io/badge/Tests-67%20Passing-brightgreen.svg)](https://github.com/ATaylorAerospace/Ion-Propulsion)

A flight grade tri language (Python, MATLAB, C++) library for gridded ion thruster modeling and mission analysis, developed for space probes and GEO satellites.

**Author: A Taylor**

[![Contact A Taylor](https://img.shields.io/badge/Contact-A%20Taylor-brightgreen?logo=mail.ru&logoColor=white)](https://ataylor.getform.com/5w8wz)

---

## 🚀 **Introduction**

This repository contains the design and simulation tools for a gridded ion thruster that meets specific requirements for space probes and GEO satellites. The suite provides parity across three languages with verified precision (`1e-6` tolerance).

**🚀 High efficiency:** Strive for a specific impulse (Isp) of at least 3,000 seconds for maneuvering and orbital raising.

**🚀 Low propellant consumption:** Achieved through ion extraction efficiency optimization and minimized grid erosion modeling.

**🚀 Long mission lifetime:** Careful material selection with grid erosion rate tracking and neutralizer performance modeling.

**🚀 High thrust-to-weight ratio:** Crucial for maximizing payload capacity and achieving desired mission objectives.

**🚀 Grid optimization:** Optimized grid design using Child-Langmuir law modeling to minimize ion divergence and maximize thrust generation.

**🚀 Neutralizer performance:** Ensures the neutralizer effectively compensates for charge imbalance created by the ion beam.

**🚀 Thermal management:** Integrated solar cell and thermal property modeling for optimal operating temperature range.

**🚀 Reliability:** Designed for radiation resistance, micrometeoroid impacts, and environmental stresses.

**🚀 Compactness and integrability:** Compact engine design for integration with spacecraft structure, power supply, and launch vehicle fairings.

**🚀 Testability and maintainability:** Comprehensive test suites across all three languages ensure performance and readiness.

---

## 🛠 **Project Scope**

The project delivers three core modules with strict cross language parity:

**🛠 dynamics:** GEO/Lagrange mission profile calculations and low thrust trajectory modeling. Includes Hohmann transfer delta-v, Lagrange point estimation, spiral transfer analysis, and low thrust transfer time computation.

**🛠 propulsion:** High-fidelity Gridded Ion Thruster (GIT) models. Includes ion extraction efficiency, beam voltage/current modeling, Child-Langmuir current limits, grid erosion rates (molybdenum sputtering), neutralizer performance, and power processing unit integration.

**🛠 optimization:** Solvers for maximizing payload fraction (Tsiolkovsky rocket equation), minimizing propellant consumption, extending mission lifetime, and finding optimal Isp for power-constrained missions.

---

## 📖 **Repository Structure**

### 📖 **python/**

Python package using Hatch build system with `astropy.units` for all physical quantities. Constructor input validation mirrors the C++ and MATLAB implementations for cross-language safety parity. Install with `pip install -e ./python`. Tests run with `pytest`.

```
python/
  pyproject.toml
  src/ion_propulsion/
    dynamics/mission_profiles.py
    propulsion/thruster.py
    optimization/solvers.py
  tests/
    test_dynamics.py
    test_propulsion.py
    test_optimization.py
```

### 📖 **matlab/**

MATLAB function-based structure with modern `arguments` validation blocks for type safety. Run tests with `runtests('matlab/tests')`.

```
matlab/
  dynamics/
    geo_transfer_delta_v.m
    lagrange_point_l1.m
    low_thrust_transfer_time.m
    spiral_delta_v.m
  propulsion/
    GriddedIonThruster.m
  optimization/
    optimal_payload_fraction.m
    propellant_mass.m
    mission_lifetime.m
    optimize_isp_for_mission.m
  tests/
    test_dynamics.m
    test_propulsion.m
    test_optimization.m
```

### 📖 **cpp/**

Standard CMake project using C++20. Built with Google Test for validation. Build instructions below.

```
cpp/
  CMakeLists.txt
  include/ion_propulsion/
    constants.hpp
    dynamics/mission_profiles.hpp
    propulsion/thruster.hpp
    optimization/solvers.hpp
  src/
    dynamics/mission_profiles.cpp
    propulsion/thruster.cpp
    optimization/solvers.cpp
  tests/
    test_dynamics.cpp
    test_propulsion.cpp
    test_optimization.cpp
```

### 📖 **docs/**

Documentation and reference imagery for the project.

---

## 🏗️ **Build Instructions**

### 🐍 Python

```bash
cd python
pip install -e ".[test]"
pytest tests/
```

### 🧮 MATLAB

```matlab
addpath(genpath('matlab'))
runtests('matlab/tests')
```

### ⚙️ C++

```bash
cd cpp
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
cd build && ctest --output-on-failure
```

---

## 🔬 **Core Physics**

All implementations share identical constants and equations:

| Equation | Formula |
|----------|---------|
| **Tsiolkovsky Rocket Equation** | `dv = Isp * g0 * ln(m0 / mf)` |
| **Child-Langmuir Law** | `J_CL = (4/9) * eps0 * sqrt(2e/mi) * V^(3/2) / d^2` |
| **Specific Impulse** | `Isp = (1/g0) * sqrt(2eVb/mi) * eta` |
| **Hohmann Transfer** | `dv1 = v_transfer_peri - v_park` |
| **Spiral Transfer** | `dv = sqrt(mu/ri) - sqrt(mu/rf)` |

### 📊 Physical Constants

| Constant | Value | Unit |
|----------|-------|------|
| `g0` | 9.80665 | m/s^2 |
| `mu_earth` | 3.986004418e14 | m^3/s^2 |
| `R_earth` | 6.371e6 | m |
| `GEO_radius` | 42164.0e3 | m |
| `epsilon_0` | 8.854187817e-12 | F/m |
| `e_charge` | 1.602176634e-19 | C |
| `m_xenon` | 2.18e-25 | kg |

Cross-language precision verified to `1e-6` tolerance.

---

## 🔧 **API Conventions**

All three language implementations enforce the same API contracts. Key conventions to note when calling functions across languages:

**🔧 `geo_transfer_delta_v(r_park_km)`** — The `r_park_km` parameter is the parking orbit **altitude above Earth's surface** in kilometres (not the full orbital radius). The function adds `R_earth` internally. For example, pass `200.0` for a 200 km LEO parking orbit.

**🔧 `child_langmuir_current()`** — Computes space-charge-limited current density using `V_total = V_screen + |V_accel|`. The accelerator grid voltage is treated as a positive magnitude in the formula regardless of the sign convention used at construction time.

**🔧 Input Validation** — All three languages validate constructor and function inputs. Beam voltage, beam current, mass flow rate, grid spacing, and propellant mass must all be positive. Python raises `ValueError`, C++ raises `std::invalid_argument`, and MATLAB uses `arguments` blocks with `{mustBePositive}`.

**🔧 `optimize_isp_for_mission()`** — The optimizer is power-constrained: thrust is computed as `F = 2·η·P / (Isp·g₀)` and the solver searches for the Isp that maximises the resulting payload fraction. Both the `power_W` and `eta` parameters are active in the objective function across all three implementations.

---

## 🧪 **Test Summary**

| Language | Framework | Tests | Status |
|----------|-----------|-------|--------|
| 🐍 Python | pytest | 36 | ✅ Passing |
| ⚙️ C++ | Google Test | 31 | ✅ Passing |
| 🧮 MATLAB | matlab.unittest | 33 | ✅ Ready |

---

## 🤝 **Contributing**

Contributions are welcome. When submitting changes, please observe these project standards:

**🤝 Cross-language parity:** Any physics function added or modified in one language must be reflected in all three (Python, C++, MATLAB) with identical constants, formulas, and API conventions. Verify results match to `1e-6` tolerance.

**🤝 Input validation:** All constructors and functions that accept physical parameters must validate inputs. Beam voltages, currents, masses, and distances must be positive. Follow the existing pattern: Python uses `ValueError`, C++ uses `std::invalid_argument`, MATLAB uses `arguments` blocks.

**🤝 Unit safety:** Python functions must return `astropy.units.Quantity` objects. C++ and MATLAB functions document units in docstrings and header comments.

**🤝 Build artifacts:** The repository includes a `.gitignore` for Python (`__pycache__`, `*.egg-info`, `.pytest_cache`), C++ (`build/`, `*.o`, `*.so`), and MATLAB (`*.mexa64`, `*.asv`) artifacts. Do not commit generated files.

**🤝 Tests:** Every new function requires unit tests in all three languages. Run the full test suite before submitting.

---

## 📋 **Changelog**

### v1.1.0 — 2026

**🔴 Bug Fixes**

- **`optimize_isp_for_mission` (Python):** Wired `power_W` and `eta` into the objective function. Previously these parameters were accepted but unused, causing the optimizer to return pure Tsiolkovsky fractions instead of power-constrained results. Now matches the C++ implementation.
- **`child_langmuir_current` (MATLAB):** Corrected voltage variable from `beam_voltage` to `screen_grid_voltage + |accel_grid_voltage|`, restoring cross-language parity with the Python and C++ implementations.

**🟡 API Alignment**

- **`geo_transfer_delta_v` (MATLAB):** Aligned input convention to take parking orbit **altitude above Earth's surface** (km), matching Python and C++. Previously took the full orbital radius (km). MATLAB test updated accordingly.

**🟡 Robustness**

- **`GriddedIonThruster` constructor (Python):** Added input validation for beam voltage, beam current, mass flow rate, grid spacing, and propellant mass. Raises `ValueError` on non-positive values, matching the guards in C++ (`std::invalid_argument`) and MATLAB (`{mustBePositive}`).

**🟢 Housekeeping**

- **`constants.hpp` (C++):** Fixed documentation comment referencing "Rust" — corrected to "MATLAB".
- **`.gitignore`:** Added repository-wide `.gitignore` covering Python, C++, MATLAB, and IDE artifacts.

---

## 📖 **References**

- Goebel, D.M. and Katz, I., *Fundamentals of Electric Propulsion: Ion and Hall Thrusters*
- Vallado, D.A., *Fundamentals of Astrodynamics and Applications*

---

## 📖 **Citation**

If you use this repository in your research, please cite it as:

```bibtex
@misc{ATaylor_IonPropulsion_2026,
  author       = {A. Taylor},
  title        = {Ion Propulsion: Tri-Language Gridded Ion Thruster Suite},
  year         = {2026},
  url          = {https://github.com/ATaylorAerospace/Ion-Propulsion/},
  note         = {Accessed: YYYY-MM-DD}
}
```

---

## 📖 **Contact**

[![Contact A Taylor](https://img.shields.io/badge/Contact-A%20Taylor-brightgreen?logo=mail.ru&logoColor=white)](https://ataylor.getform.com/5w8wz)

---

## 📖 **License**

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

