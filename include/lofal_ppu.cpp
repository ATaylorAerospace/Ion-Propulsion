```cpp
#ifndef THRUSTER_SPACE_THRUSTER
#define THRUSTER_SPACE_THRUSTER

#include <iostream>
#include "Thruster.hpp"

namespace ThrusterSpace {

class Thruster {
public:
    Thruster(double thrust, double Isp, double massFlowRate, double propellantDensity, double nozzleArea, double thrustToWeightRatio, double efficiency) :
            m_thrust(thrust),
            m_Isp(Isp),
            m_massFlowRate(massFlowRate),
            m_propellantDensity(propellantDensity),
            m_nozzleArea(nozzleArea),
            m_thrustToWeightRatio(thrustToWeightRatio),
            m_efficiency(efficiency) {
    }

    double GetThrust() const {
        return m_thrust;
    }

    double GetIsp() const {
        return m_Isp;
    }

    double GetMassFlowRate() const {
        return m_massFlowRate;
    }

    double GetPropellantDensity() const {
        return m_propellantDensity;
    }

    double GetNozzleArea() const {
        return m_nozzleArea;
    }

    double GetThrustToWeightRatio() const {
        return m_thrustToWeightRatio;
    }

    double GetEfficiency() const {
        return m_efficiency;
    }

    void SetThrust(double thrust) {
        m_thrust = thrust;
    }

    void SetIsp(double isp) {
        m_Isp = isp;
    }

    void SetMassFlowRate(double massFlowRate) {
        m_massFlowRate = massFlowRate;
    }

    void SetPropellantDensity(double propellantDensity) {
        m_propellantDensity = propellantDensity;
    }

    void SetNozzleArea(double nozzleArea) {
        m_nozzleArea = nozzleArea;
    }

    void SetThrustToWeightRatio(double thrustToWeightRatio) {
        m_thrustToWeightRatio = thrustToWeightRatio;
    }

    void SetEfficiency(double efficiency) {
        m_efficiency = efficiency;
    }

private:
    double m_thrust;
    double m_Isp;
    double m_massFlowRate;
    double m_propellantDensity;
    double m_nozzleArea;
    double m_thrustToWeightRatio;
    double m_efficiency;
};

}  // namespace ThrusterSpace

#endif  // THRUSTER_SPACE_THRUSTER_H
```

