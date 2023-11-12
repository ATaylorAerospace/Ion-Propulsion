import math

class KeplerVelocity:
    def __init__(self, gravitational_parameter):
        """
        Initialize the KeplerVelocity class with the gravitational parameter of the central body.

        Parameters:
        gravitational_parameter (float): The gravitational parameter (GM) of the central body.
        """
        self.gravitational_parameter = gravitational_parameter

    def calculate_velocity(self, semi_major_axis, distance):
        """
        Calculate the velocity of an object in a Keplerian orbit.

        Parameters:
        semi_major_axis (float): The semi-major axis of the orbit.
        distance (float): The distance from the central body to the orbiting object.

        Returns:
        float: The velocity of the object in the orbit.
        """
        try:
            # Ensure that the semi-major axis and distance are positive values.
            if semi_major_axis <= 0 or distance <= 0:
                raise ValueError("Semi-major axis and distance must be positive values.")

            # Calculate the velocity using the vis-viva equation.
            velocity = math.sqrt(self.gravitational_parameter * (
                (2 / distance) - (1 / semi_major_axis)
            ))
            return velocity

        except ValueError as e:
            print(f"Error: {e}")
            return None

# Example usage:
if __name__ == "__main__":
    # Define the gravitational parameter of the central body (e.g., Earth).
    central_body_gravitational_parameter = 398600.4418  # Earth's gravitational parameter in km^3/s^2

    # Create an instance of the KeplerVelocity class.
    kepler_velocity_calculator = KeplerVelocity(central_body_gravitational_parameter)

    # Define the semi-major axis and distance for an orbit (in kilometers).
    semi_major_axis = 7000  # Semi-major axis of the orbit
    distance = 8000  # Distance from the central body to the orbiting object

    # Calculate the velocity of the object in the orbit.
    velocity = kepler_velocity_calculator.calculate_velocity(semi_major_axis, distance)

    if velocity is not None:
        print(f"Velocity of the object in the orbit: {velocity} km/s")

