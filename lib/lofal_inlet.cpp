```cpp
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <vector>

#define MAX_THROTTLE 1000
#define MIN_THROTTLE 10
#define MAX_VELOCITY 100
#define MIN_VELOCITY 5

using namespace std;

struct Inlet {
    double throttle;
    double velocity;
};

// Returns a random number between min and max
double random(double min, double max) {
    return min + (rand() % (max - min + 1)) / (RAND_MAX + 1);
}

// Generates a random Inlet
Inlet generate_inlet() {
    Inlet inlet;
    inlet.throttle = random(MIN_THROTTLE, MAX_THROTTLE);
    inlet.velocity = random(MIN_VELOCITY, MAX_VELOCITY);
    return inlet;
}

int main() {
    srand(time(NULL));

    // Generate a list of Inlet objects
    vector<Inlet> inlets;
    for (int i = 0; i < 10; i++) {
        inlets.push_back(generate_inlet());
    }

    // Sort the inlets by throttle, then by velocity
    sort(inlets.begin(), inlets.end(), [](Inlet a, Inlet b) {
        if (fabs(a.throttle - b.throttle) > 1e-9) {
            return a.throttle < b.throttle;
        } else {
            return a.velocity < b.velocity;
        }
    });

    // Print the inlets
    for (const auto& inlet : inlets) {
        printf("Inlet: throttle = %f, velocity = %f\n", inlet.throttle, inlet.velocity);
    }

    return 0;
}
