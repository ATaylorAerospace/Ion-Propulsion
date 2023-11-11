class generateOrbitalElements:
    def __init__(self, a, ecc, inc, time):
        self.a = a
        self.ecc = ecc
        self.inc = inc
        self.time = time

    def generateOrbitalElements(self):
        mean_motion = 0.917 * (self.a ** 3) / (1 + self.ecc * math.cos(self.inc))
        orbital_elements = {
            "semi-major axis": self.a,
            "eccentricity": self.ecc,
            "inclination": self.inc,
            "mean anomaly": self.time,
            "mean motion": mean_motion
        }
        return orbital_elements
