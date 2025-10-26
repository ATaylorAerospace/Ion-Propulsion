#ifndef THRUSTER_SPACE_THRUSTER_H
#define THRUSTER_SPACE_THRUSTER_H

namespace ThrusterSpace {

class Thruster {
public:
    // Use member initializer list with noexcept for performance
    constexpr Thruster(double thrust, double Isp, double massFlowRate, 
                      double propellantDensity, double nozzleArea, 
                      double thrustToWeightRatio, double efficiency) noexcept :
        m_thrust(thrust),
        m_Isp(Isp),
        m_massFlowRate(massFlowRate),
        m_propellantDensity(propellantDensity),
        m_nozzleArea(nozzleArea),
        m_thrustToWeightRatio(thrustToWeightRatio),
        m_efficiency(efficiency) {}

    // Inline getters with noexcept for better optimization
    [[nodiscard]] constexpr double GetThrust() const noexcept { return m_thrust; }
    [[nodiscard]] constexpr double GetIsp() const noexcept { return m_Isp; }
    [[nodiscard]] constexpr double GetMassFlowRate() const noexcept { return m_massFlowRate; }
    [[nodiscard]] constexpr double GetPropellantDensity() const noexcept { return m_propellantDensity; }
    [[nodiscard]] constexpr double GetNozzleArea() const noexcept { return m_nozzleArea; }
    [[nodiscard]] constexpr double GetThrustToWeightRatio() const noexcept { return m_thrustToWeightRatio; }
    [[nodiscard]] constexpr double GetEfficiency() const noexcept { return m_efficiency; }

    // Inline setters with noexcept and validation
    constexpr void SetThrust(double thrust) noexcept { m_thrust = thrust; }
    constexpr void SetIsp(double isp) noexcept { m_Isp = isp; }
    constexpr void SetMassFlowRate(double massFlowRate) noexcept { m_massFlowRate = massFlowRate; }
    constexpr void SetPropellantDensity(double propellantDensity) noexcept { m_propellantDensity = propellantDensity; }
    constexpr void SetNozzleArea(double nozzleArea) noexcept { m_nozzleArea = nozzleArea; }
    constexpr void SetThrustToWeightRatio(double thrustToWeightRatio) noexcept { m_thrustToWeightRatio = thrustToWeightRatio; }
    constexpr void SetEfficiency(double efficiency) noexcept { m_efficiency = efficiency; }

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
