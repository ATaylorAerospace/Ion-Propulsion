# Author: A Taylor | Purpose: Top-level package for ion propulsion library | Ref: Goebel/Katz/Vallado

from ion_propulsion import dynamics
from ion_propulsion import propulsion
from ion_propulsion import optimization

__version__ = "1.2.0"

__all__ = ["dynamics", "propulsion", "optimization", "__version__"]
