class RFITPerformanceCalculator:
    def __init__(self, specific_impulse, thrust_power, efficiency):
        """
        Initialize the RFITPerformanceCalculator class with specific parameters.

        Parameters:
        specific_impulse (float): The specific impulse of the ion thruster (in seconds).
        thrust_power (float): The thrust power of the ion thruster (in watts).
        efficiency (float): The efficiency of the ion thruster (as a decimal).
        """
        self.specific_impulse = specific_impulse
        self.thrust_power = thrust_power
        self.efficiency = efficiency

    def calculate_performance_threshold(self):
        """
        Calculate the performance threshold for the RF ion thruster.

        Returns:
        float: The performance threshold (in newtons).
        """
        try:
            # Ensure that specific impulse and thrust power are positive values.
            if self.specific_impulse <= 0 or self.thrust_power <= 0:
                raise ValueError("Specific impulse and thrust power must be positive values.")

            # Calculate the performance threshold using the thrust equation.
            performance_threshold = (2 * self.thrust_power * self.efficiency) / self.specific_impulse
            return performance_threshold

        except ValueError as e:
            print(f"Error: {e}")
            return None

# Example usage:
if __name__ == "__main__":
    # Define specific parameters for the RF ion thruster.
    specific_impulse = 3000  # Specific impulse in seconds
    thrust_power = 4000  # Thrust power in watts
    efficiency = 0.75  # Efficiency of the ion thruster (85%)

    # Create an instance of the RFITPerformanceCalculator class.
    rfit_calculator = RFITPerformanceCalculator(specific_impulse, thrust_power, efficiency)

    # Calculate the performance threshold.
    threshold = rfit_calculator.calculate_performance_threshold()

    if threshold is not None:
        print(f"Performance threshold for RF ion thruster: {threshold} newtons")
