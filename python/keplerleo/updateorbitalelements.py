class update orbitalelements:
    def __init__(self, a, ecc, inc, time):
        self.a = a
        self.ecc = ecc
        self.inc = inc
        self.time = time

    def update orbitalelements(self, new_elements):
        self.a = new_elements["semi-major axis"]
        self.ecc = new_elements["eccentricity"]
        self.inc = new_elements["inclination"]
        self.time = new_elements["mean anomaly"]
