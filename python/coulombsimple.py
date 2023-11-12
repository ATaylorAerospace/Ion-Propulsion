# F is the magnitude of the force between the charges,
# q1 and q2 are the amounts of charge,
# r is the distance between the centers of the two charges,
# k is Coulomb’s constant, approximately equal to 8.99×109N(m2/C2)

import math

def calculate_coulomb_force(q1, q2, r):
    k = 8.99e9
    return k * abs(q1 * q2) / (r * r)

q1 = float(input("Enter the first charge (in Coulombs): "))
q2 = float(input("Enter the second charge (in Coulombs): "))
r = float(input("Enter the distance between the charges (in meters): "))

force = calculate_coulomb_force(q1, q2, r)

print("The Coulomb force between the charges is: ", force, "N")


