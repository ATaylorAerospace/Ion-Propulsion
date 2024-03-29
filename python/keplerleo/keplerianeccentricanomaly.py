class calculate keplerianEccentricAnomaly:
    def __init__(self, a, ecc, inc, time):
        self.a = a
        self.ecc = ecc
        self.inc = inc
        self.time = time

    def calculate keplerianEccentricAnomaly(self, ecc):
        # Formula: Eccentric Anomaly = ecc * (1 + ecc) * (1 + ecc * cos(inc)) ** 0.5
        eccentric_anomaly = ecc * (1 + ecc) * (1 + ecc * math.cos(self.inc)) ** 0.5
        return eccentric_anomaly
