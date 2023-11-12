class calculate keplerianmeanmotion:
    def __init__(self, a, ecc, inc, time):
        self.a = a
        self.ecc = ecc
        self.inc = inc
        self.time = time

    def calculate keplerianmeanmotion(self):
        # Formula: Mean Motion = 0.917 * (a ** 3) / (1 + ecc * cos(inc))
        mean_motion = 0.917 * (self.a ** 3) / (1 + self.ecc * math.cos(self.inc))
        return mean_motion
