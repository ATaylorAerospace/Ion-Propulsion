# Ion-Propulsion
Initial Ion Propulsion Repository - Anfängliches Ionenantriebs-Repository
This code implements a solver for Kepler's Equation to calculate the position and motion of an orbiting body over time.

KeplerLEOVelocity Class

The KeplerLEOVelocity class represents the orbital elements that define the orientation of an orbiting body without changing the plane of the orbit. It takes in parameters like semi-major axis, orbital period, eccentricity, etc. and calculates quantities like the mean anomaly and eccentric anomaly to determine the position over time.

The key methods are:

init : Constructor that initializes the orbital elements
meanAnomaly : Calculates mean anomaly based on time
radius : Calculates orbit radius based on time and eccentric anomaly
xyzPos : Calculates x,y,z position based on time
It uses the MarkleyKESolver class to numerically solve Kepler's Equation and find the eccentric anomaly from the mean anomaly and eccentricity.

The output is the orbit radius and 3D position coordinates at given points in time. This allows determining the location of an orbiting body given its orbital elements.

MarkleyKESolver Class

This class provides a solver to numerically calculate the eccentric anomaly E given the mean anomaly M and eccentricity e by implementing an algorithm from Markley (1995).

It has methods to:

Solve the equations defining E iteratively
Test precision over random inputs
Get the eccentric anomaly for given M and e
The solver is used by the KeplerLEOVelocity class to find E needed to determine the orbit position. It takes in M and e and returns the solved E.

Overall, these classes allow calculating the position and motion of an orbiting body over time given its orbital parameters by solving Kepler's Equation. The output is the changing x,y,z coordinates as the body moves along its orbit.



