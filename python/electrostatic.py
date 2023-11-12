class GEOIonThrustCalculator:
    def __init__(self, specific_impulse, mass_flow_rate):
        """
        Initialize the GEOIonThrustCalculator class with specific parameters.

        Parameters:
        specific_impulse (float): The specific impulse of the ion thruster (in seconds).
        mass_flow_rate (float): The mass flow rate of propellant (in kilograms per second).
        """
        self.specific_impulse = specific_impulse
        self.mass_flow_rate = mass_flow_rate

    def calculate_ion_thrust(self):
        """
        Calculate the ion thrust in GEO using the rocket equation.

        Returns:
        float: The ion thrust (in newtons).
        """
        try:
            # Ensure that specific impulse and mass flow rate are positive values.
            if self.specific_impulse <= 0 or self.mass_flow_rate <= 0:
                raise ValueError("Specific impulse and mass flow rate must be positive values.")

            # Calculate the ion thrust using the rocket equation.
            ion_thrust = self.specific_impulse * self.mass_flow_rate * 9.81  # 9.81 m/s^2 is Earth's gravity
            return ion_thrust

        except ValueError as e:
            print(f"Error: {e}")
            return None

# Example usage:
if __name__ == "__main__":
    # Define specific parameters for the ion thruster in GEO.
    specific_impulse = 3000  # Specific impulse in seconds
    mass_flow_rate = 0.001  # Mass flow rate in kilograms per second

    # Create an instance of the GEOIonThrustCalculator class.
    geo_ion_thrust_calculator = GEOIonThrustCalculator(specific_impulse, mass_flow_rate)

    # Calculate the ion thrust in GEO.
    ion_thrust = geo_ion_thrust_calculator.calculate_ion_thrust()

    if ion_thrust is not None:
        print(f"Ion thrust in GEO: {ion_thrust} newtons")
