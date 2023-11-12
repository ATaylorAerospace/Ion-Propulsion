# F is the Lorentz force,
# q is the charge of the particle,
# E is the electric field,
# v is the velocity of the particle, and
# B is the magnetic field

import numpy as np

def calculate_lorentz_force(q, E, v, B):
    return q * (E + np.cross(v, B))

q = float(input("Enter the charge of the particle (in Coulombs): "))
E = np.array(list(map(float, input("Enter the electric field (as space-separated values for i, j, k components): ").split())))
v = np.array(list(map(float, input("Enter the velocity of the particle (as space-separated values for i, j, k components): ").split())))
B = np.array(list(map(float, input("Enter the magnetic field (as space-separated values for i, j, k components): ").split())))

force = calculate_lorentz_force(q, E, v, B)

print("The Lorentz force on the particle is: ", force, "N")
