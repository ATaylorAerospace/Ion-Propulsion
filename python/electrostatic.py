class GEOIonThrustCalculator:
    def __init__(self, specific_impulse, mass_flow_rate):
        """
        Initialize the calculator with specific impulse and mass flow rate.
        """
        if specific_impulse <= 0 or mass_flow_rate <= 0:
            raise ValueError("Specific impulse and mass flow rate must be positive values.")
        self.specific_impulse = specific_impulse
        self.mass_flow_rate = mass_flow_rate

    def calculate_ion_thrust(self):
        """
        Calculate and return the ion thrust in GEO.
        """
        return self.specific_impulse * self.mass_flow_rate * 9.81  # Earth's gravity in m/s^2


# Example usage:
if __name__ == "__main__":
    try:
        # Define specific parameters for the ion thruster.
        specific_impulse = 3000  # Specific impulse in seconds
        mass_flow_rate = 0.001  # Mass flow rate in kilograms per second

        # Calculate the ion thrust in GEO.
        calculator = GEOIonThrustCalculator(specific_impulse, mass_flow_rate)
        ion_thrust = calculator.calculate_ion_thrust()
        print(f"Ion thrust in GEO: {ion_thrust:.2f} newtons")
    except ValueError as e:
        print(f"Error: {e}")
