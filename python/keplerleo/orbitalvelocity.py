class calculate orbitalvelocity:
    def __init__(self, a, ecc, inc, time):
        self.a = a
        self.ecc = ecc
        self.inc = inc
        self.time = time

    def calculate orbitalvelocity(self):
        # Formula: Orbital Velocity = (GM / a)^(1/2)
        orbital_velocity = (G * self.a ** 2) ** (1/2)
        return orbital_velocity
