#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <complex>
#include <map>
#include <set>
#include <bitset>

#include "cell.h"
#include "solar_cell.h"
#include "electric_powered_mass_thruster.h"
#include "satellite.h"

using namespace std;

Cell::Cell(const double &width, const double &height, const double &thickness, const double &mass, const double &specific_heat, const double &thermal_conductivity, const double &emissivity, const double &reflectivity) :
    width(width),
    height(height),
    thickness(thickness),
    mass(mass),
    specific_heat(specific_heat),
    thermal_conductivity(thermal_conductivity),
    emissivity(emissivity),
    reflectivity(reflectivity)
{
    // Initialize the solar cell
    SolarCell *solar_cell = new SolarCell(width, height, specific_heat, thermal_conductivity, emissivity);
    solar_cell->set_reflectivity(reflectivity);
    this->solar_cell = solar_cell;

    // Initialize the electric powered mass thruster
    ElectricPoweredMassThruster *electric_powered_mass_thruster = new ElectricPoweredMassThruster(width, height, thickness, mass, specific_heat, thermal_conductivity, emissivity);
    electric_powered_mass_thruster->set_reflectivity(reflectivity);
    this->electric_powered_mass_thruster = electric_powered_mass_thruster;

    // Initialize the satellite
    Satellite *satellite = new Satellite(width, height, mass, specific_heat, thermal_conductivity, emissivity);
    satellite->set_reflectivity(reflectivity);
    this->satellite = satellite;
}

Cell::~Cell()
{
    delete solar_cell;
    delete electric_powered_mass_thruster;
    delete satellite;
}

void Cell::set_width(const double &width)
{
    this->width = width;
    solar_cell->set_width(width);
    electric_powered_mass_thruster->set_width(width);
    satellite->set_width(width);
}

void Cell::set_height(const double &height)
{
    this->height = height;
    solar_cell->set_height(height);
    electric_powered_mass_thruster->set_height(height);
    satellite->set_height(height);
}

void Cell::set_thickness(const double &thickness)
{
    this->thickness = thickness;
    electric_powered_mass_thruster->set_thickness(thickness);
}

void Cell::set_mass(const double &mass)
{
    this->mass = mass;
    satellite->set_mass(mass);
}

void Cell::set_specific_heat(const double &specific_heat)
{
    this->specific_heat = specific_heat;
    solar_cell->set_specific_heat(specific_heat);
    electric_powered_mass_thruster->set_specific_heat(specific_heat);
    satellite->set_specific_heat(specific_heat);
}

void Cell::set_thermal_conductivity(const double &thermal_conductivity)
{
    this->thermal_conductivity = thermal_conductivity;
    solar_cell->set_thermal_conductivity(thermal_conductivity);
    electric_powered_mass_thruster->set_thermal_conductivity(thermal_conductivity);
}

void Cell::set_emissivity(const double &emissivity)
{
    this->emissivity = emissivity;
    solar_cell->set_emissivity(emissivity);
    electric_powered_mass_thruster->set_emissivity(emissivity);
    satellite->set_emissivity(emissivity);
}

void Cell::set_reflectivity(const double &reflectivity)
{
    this->reflectivity = reflectivity;
    solar_cell->set_reflectivity(reflectivity);
    electric_powered_mass_thruster->set_reflectivity(reflectivity);
    satellite->set_reflectivity(reflectivity);
}

double Cell::get_width() const
{
    return width;
}

double Cell::get_height() const
{
    return height;
}

double Cell::get_thickness() const
{
    return thickness;
}

double Cell::get_mass() const
{
    return mass;
}

double Cell::get_specific_heat() const
{
    return specific_heat;
}

double Cell::get_thermal_conductivity() const
{
    return thermal_conductivity;
}

double Cell::get_emissivity() const
{
    return emissivity;
}

double Cell::get_reflectivity() const
{
    return reflectivity;
}

SolarCell *Cell::get_solar_cell() const
{
    return solar_cell;
}

ElectricPoweredMassThruster *Cell::get_electric_powered_mass_thruster() const
{
    return electric_powered_mass_thruster;
}

Satellite *Cell::get_satellite() const
{
    return satellite;
}
