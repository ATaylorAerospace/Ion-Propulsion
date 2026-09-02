# ion-propulsion

Flight-grade gridded ion thruster modeling and electric propulsion mission analysis, with `astropy.units` on every physical quantity.

This is the Python implementation of the tri-language Ion-Propulsion suite. The C++ and MATLAB implementations, the shared physics, the NSTAR benchmark, and the full documentation live in the repository README:

https://github.com/ATaylorAerospace/Ion-Propulsion

```bash
pip install -e ".[test]"
pytest
```

```python
from ion_propulsion.propulsion import GriddedIonThruster

t = GriddedIonThruster(1100.0, 1.76, 1100.0, -180.0, 3.0e-6)   # NSTAR at full power
print(t.specific_impulse_s, t.thrust_N.to("mN"))                # 3273.07 s, 96.29 mN
```

Author: A Taylor. MIT License.
