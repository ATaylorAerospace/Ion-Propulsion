class calculate trueanomaly:
    def __init__(self, a, ecc, inc, time):
        self.a = a
        self.ecc = ecc
        self.inc = inc
        self.time = time

    def calculate trueanomaly(self, ecc):
        # Formula: True Anomaly = M + ecc * (1 - ecc) * sin(inc)
        true_anomaly = self.time + ecc * (1 - ecc) * math.sin(self.inc)
        return true_anomaly
