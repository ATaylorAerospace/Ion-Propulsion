# Coulomb's Law: Calculate the force between two charges
# F = k * |q1 * q2| / r^2

def calculate_coulomb_force(q1, q2, r):
    """Calculate the Coulomb force between two charges."""
    k = 8.99e9  # Coulomb's constant in N·m²/C²
    return k * abs(q1 * q2) / (r ** 2)

def main():
    try:
        # Input charges and distance
        q1 = float(input("Enter the first charge (in Coulombs): "))
        q2 = float(input("Enter the second charge (in Coulombs): "))
        r = float(input("Enter the distance between the charges (in meters): "))

        if r <= 0:
            raise ValueError("Distance must be greater than zero.")

        # Calculate and display the force
        force = calculate_coulomb_force(q1, q2, r)
        print(f"The Coulomb force between the charges is: {force:.2e} N")

    except ValueError as e:
        print(f"Invalid input: {e}")

if __name__ == "__main__":
    main()



