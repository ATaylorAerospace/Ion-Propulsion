#ifndef INLET_H
#define INLET_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MAX_THROTTLE 1000
#define MIN_THROTTLE 5
#define MAX_VELOCITY 100
#define MIN_VELOCITY 10

using namespace std;

struct Inlet {
    double throttle;
    double velocity;
};

#endif // INLET_H
