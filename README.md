![Geosat](docs/geosats.png)


# Introduction

This repository contains the design for an ion thruster that meets specific requirements for a GEO satellite. The design will focus on the following:

**- The ion thruster must be able to provide a specific impulse of at least 1500 seconds in GEO.**
**- The ion thruster must be able to operate at a power level of at least 120 watts.**
**- The ion thruster must be able to withstand a maximum discharge voltage of at least 10,000 volts.**

The project will be completed in several phases. The first phase will involve the design of the ion thruster itself. This section will discuss the selection of the appropriate ion thruster geometry and the development of a detailed model of the ion thruster. The second phase will involve the design of the power supply for the ion thruster. This section will discuss the selection of the appropriate power supply architecture and the development of a detailed model of the power supply. The third phase will involve the design of the control system for the ion thruster. This section will discuss the selection of the appropriate control system architecture and the development of a detailed model of the control system.

# Documentation

The documentation for the project will be contained in the docs folder. This will include detailed designs of the ion thruster, the power supply, and the control system. It will also include documentation of the thruster testing process and the results of the testing. This section will contain information about the overall design of the thruster, including a discussion of the research that went into the design and the trade-offs made during the design process.

# Include

The include folder will contain the header files that will be used by the C++ files in the python and lib folders. These headers will define the data types and function declarations that will be used by the C++ code.

# Python

The python folder will contain the Python scripts that will be used to control the ion thruster, power supply, and GEO calculations. These scripts will be used to test the ion thruster and the power supply and GEO simulations. The Python scripts will call the C++ libraries to control the thruster and power supply. The Python scripts will also perform the GEO calculations to determine the specific impulse of the ion thruster.

# Tests

The tests folder will contain the tests that will be used to verify the functionality of the ion thruster and the power supply. These tests will include both unit tests and integration tests. The tests will be written in Python and will call the C++ libraries to test the thruster and power supply. The tests will also perform GEO calculations to verify the specific impulse of the ion thruster.

# Lib

The lib folder will contain the C++ libraries that will be used by the Python scripts in the python folder. These libraries will define the data types and function implementations that will be used by the Python code. The lib folder will contain the following libraries:

- A C++ library that will be used to control the ion thruster. This library will be called by the Python scripts to control the ion thruster.
- A C++ library that will be used to control the power supply. This library will be called by the Python scripts to control the power supply.
- A C++ library that will be used to communicate with the control system. This library will be called by the Python scripts to communicate with the control system.

# Source

This folder contains the C++ source code files for the propulsion system, thruster, nozzle, and fuels. 
