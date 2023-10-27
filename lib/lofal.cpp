nclude <stdio.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <vector>
#include <iostream>

#include "Thruster.hpp"

namespace ThrusterSpace {

Thruster::Thruster(double thrust, double Isp, double massFlowRate, double propellantDensity, double nozzleArea, double thrustToWeightRatio, double efficiency) :
    m_thrust(thrust),
    m_Isp(Isp),
    m_massFlowRate(massFlowRate),
    m_propellantDensity(propellantDensity),
    m_nozzleArea(nozzleArea),
    m_thrustToWeightRatio(thrustToWeightRatio),
    m_efficiency(efficiency)
{
}

double Thruster::GetThrust() const {
    return m_thrust;
}

double Thruster::GetIsp() const {
    return m_Isp;
}

double Thruster::GetMassFlowRate() const {
    return m_massFlowRate;
}

double Thruster::GetPropellantDensity() const {
    return m_propellantDensity;
}

double Thruster::GetNozzleArea() const {
    return m_nozzleArea;
}

double Thruster::GetThrustToWeightRatio() const {
    return m_thrustToWeightRatio;
}

double Thruster::Get Efficiency() const {
    return m_efficiency;
}

void Thruster::SetThrust(double thrust) {
    m_thrust = thrust;
}

void Thruster::SetIsp(double isp) {
    m_Isp = isp;
}

void Thruster::SetMassFlowRate(double massFlowRate) {
    m_massFlowRate = massFlowRate;
}

void Thruster::SetPropellantDensity(double propellantDensity) {
    m_propellantDensity = propellantDensity;
}

void Thruster::SetNozzleArea(double nozzleArea) {
    m_nozzleArea = nozzleArea;
}

void Thruster::SetThrustToWeightRatio(double thrustToWeightRatio) {
    m_thrustToWeightRatio = thrustToWeightRatio;
}

void Thruster::SetEfficiency(double efficiency) {
    m_efficiency = efficiency;
}

}  // namespace ThrusterSpace
