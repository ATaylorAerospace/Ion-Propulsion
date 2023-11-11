# KeplerLeo
Initial Space Propulsion Repository - Anfängliches Ionenantriebs-Repository
This code implements a solver for Kepler's Equation to calculate the position and motion of an orbiting body over time.

KeplerLEOVelocity Class

The KeplerLEOVelocity class provides methods for calculating the orbital velocity of a low Earth orbit (LEO) satellite. The class defines several key methods that are essential for understanding and working with satellite orbits.

**calculateKeplerianMeanMotion:** This method calculates the Keplerian mean motion of a satellite, which is the average rate at which the satellite orbits its central body. The mean motion is an important parameter for understanding the long-term behavior of a satellite's orbit.

**calculateKeplerianEccentricAnomaly:** This method calculates the Keplerian eccentric anomaly of a satellite, which is a mathematical quantity that describes the position of a satellite along its orbit. The eccentric anomaly is useful for converting between the true anomaly, which is the angle between the satellite's position and the periapsis of its orbit, and the mean anomaly.

**calculateTrueAnomaly:** This method calculates the true anomaly of a satellite, which is the angle between the satellite's position and the periapsis of its orbit. The true anomaly is the most commonly used measure of a satellite's position along its orbit.

**calculateOrbitalVelocity:** This method calculates the orbital velocity of a satellite, which is the speed at which the satellite is moving along its orbit. The orbital velocity is a critical parameter for understanding the satellite's behavior and for designing spacecraft trajectories.

**calculatePosition:** This method calculates the position of a satellite in three-dimensional space, given its orbital parameters and a time reference. The position is expressed in terms of either Cartesian coordinates (x, y, z) or spherical coordinates (longitude, latitude, altitude).

**generateOrbitalElements:** This method generates a set of orbital elements for a satellite, including the semi-major axis, eccentricity, inclination, right ascension of the ascending node, argument of periapsis, and true anomaly. The orbital elements provide a complete description of the satellite's orbit.

**updateOrbitalElements:** This method updates the orbital elements of a satellite based on new measurements of its position and velocity. This is important for tracking the satellite's orbit and predicting its future position.



