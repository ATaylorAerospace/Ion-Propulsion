import numpy as np

def calculate_lorentz_force(q, E, v, B):
    """
    Calculate the Lorentz force on a charged particle.

    Parameters:
    - q: Charge of the particle (float)
    - E: Electric field vector (numpy array)
    - v: Velocity vector of the particle (numpy array)
    - B: Magnetic field vector (numpy array)

    Returns:
    - Lorentz force vector (numpy array)
    """
    return q * (E + np.cross(v, B))

def get_vector_input(prompt):
    """
    Prompt the user for a 3D vector input.

    Parameters:
    - prompt: The input prompt message (str)

    Returns:
    - 3D vector as a numpy array
    """
    while True:
        try:
            values = list(map(float, input(prompt).split()))
            if len(values) == 3:
                return np.array(values)
            else:
                print("Please enter exactly three components (i, j, k).")
        except ValueError:
            print("Invalid input. Please enter numeric values.")

def main():
    print("Lorentz Force Calculator")
    try:
        q = float(input("Enter the charge of the particle (in Coulombs): "))
        E = get_vector_input("Enter the electric field (i, j, k): ")
        v = get_vector_input("Enter the velocity of the particle (i, j, k): ")
        B = get_vector_input("Enter the magnetic field (i, j, k): ")
        
        force = calculate_lorentz_force(q, E, v, B)
        print(f"The Lorentz force on the particle is: {force} N")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()
