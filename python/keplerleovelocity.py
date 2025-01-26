import math

class KeplerVelocity:
    def __init__(self, gravitational_parameter: float):
        """
        Initialize the KeplerVelocity class with the gravitational parameter of the central body.

        Parameters:
        gravitational_parameter (float): The gravitational parameter (GM) of the central body.
        """
        if gravitational_parameter <= 0:
            raise ValueError("Gravitational parameter must be a positive value.")
        self.gravitational_parameter = gravitational_parameter

    def calculate_velocity(self, semi_major_axis: float, distance: float) -> float:
        """
        Calculate the velocity of an object in a Keplerian orbit.

        Parameters:
        semi_major_axis (float): The semi-major axis of the orbit (km).
        distance (float): The distance from the central body to the orbiting object (km).

        Returns:
        float: The velocity of the object in the orbit (km/s).
        """
        if semi_major_axis <= 0 or distance <= 0:
            raise ValueError("Semi-major axis and distance must be positive values.")

        # Use the vis-viva equation to calculate velocity.
        return math.sqrt(
            self.gravitational_parameter * ((2 / distance) - (1 / semi_major_axis))
        )

# Example usage:
if __name__ == "__main__":
    # Gravitational parameter of Earth (in km^3/s^2)
    GRAVITATIONAL_PARAMETER = 398600.4418

    # Create an instance of the KeplerVelocity class.
    kepler_velocity_calculator = KeplerVelocity(GRAVITATIONAL_PARAMETER)

    # Define orbital parameters (in kilometers).
    semi_major_axis = 7000  # Semi-major axis of the orbit
    distance = 8000  # Distance from the central body to the orbiting object

    try:
        # Calculate the orbital velocity.
        velocity = kepler_velocity_calculator.calculate_velocity(semi_major_axis, distance)
        print(f"Velocity of the object in the orbit: {velocity:.2f} km/s")
    except ValueError as e:
        print(f"Input error: {e}")


