class calculate position:
    def __init__(self, a, ecc, inc, time):
        self.a = a
        self.ecc = ecc
        self.inc = inc
        self.time = time

    def calculate position(self):
        # Formula: Position = a * (1 - ecc * cos(true_anomaly)) + GM / a * (1 - ecc ** 2) ** 0.5
        position = self.a * (1 - self.ecc * math.cos(self.time)) + G * (1 - self.ecc ** 2) ** (1/2)
        return position
