#pragma once
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>

class InletGenerator {
public:
    // Constants for inlet parameters
    static constexpr double MAX_THROTTLE = 1000.0;
    static constexpr double MIN_THROTTLE = 10.5;
    static constexpr double MAX_VELOCITY = 100.0;
    static constexpr double MIN_VELOCITY = 5.0;

    // Struct to represent inlet characteristics
    struct Inlet {
        double throttle;
        double velocity;
    };

    // Constructor with optional random seed
    explicit InletGenerator(unsigned int seed = std::random_device{}()) 
        : generator_(seed), 
          throttle_dist_(MIN_THROTTLE, MAX_THROTTLE),
          velocity_dist_(MIN_VELOCITY, MAX_VELOCITY) {}

    // Generate a single inlet with controlled randomness
    Inlet generateInlet() {
        return {
            throttle_dist_(generator_),
            velocity_dist_(generator_)
        };
    }

    // Generate multiple inlets
    std::vector<Inlet> generateInlets(size_t count) {
        std::vector<Inlet> inlets;
        inlets.reserve(count);  // Optimize memory allocation
        
        for (size_t i = 0; i < count; ++i) {
            inlets.push_back(generateInlet());
        }

        // Sort inlets by throttle, with stable velocity secondary sort
        std::stable_sort(inlets.begin(), inlets.end(), 
            [](const Inlet& a, const Inlet& b) {
                // Use epsilon for floating-point comparison
                const double epsilon = 1e-9;
                if (std::abs(a.throttle - b.throttle) > epsilon) {
                    return a.throttle < b.throttle;
                }
                return a.velocity < b.velocity;
            }
        );

        return inlets;
    }

    // Print inlet details
    static void printInlets(const std::vector<Inlet>& inlets) {
        for (const auto& inlet : inlets) {
            std::cout << "Inlet: throttle = " << inlet.throttle 
                      << ", velocity = " << inlet.velocity << std::endl;
        }
    }

private:
    // Use Mersenne Twister for better random number generation
    std::mt19937 generator_;
    std::uniform_real_distribution<> throttle_dist_;
    std::uniform_real_distribution<> velocity_dist_;
};

// Example main function demonstrating usage
int main() {
    InletGenerator generator;
    
    // Generate 10 inlets
    auto inlets = generator.generateInlets(10);
    
    // Print generated and sorted inlets
    InletGenerator::printInlets(inlets);

    return 0;
}
