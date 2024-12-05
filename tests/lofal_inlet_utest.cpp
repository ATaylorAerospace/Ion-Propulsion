#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "inlet_generator.hpp"

TEST_CASE("Inlet Generator Functionality", "[InletGenerator]") {
    SECTION("Single Inlet Generation") {
        InletGenerator generator;
        auto inlet = generator.generateInlet();

        REQUIRE(inlet.throttle >= InletGenerator::MIN_THROTTLE);
        REQUIRE(inlet.throttle <= InletGenerator::MAX_THROTTLE);
        REQUIRE(inlet.velocity >= InletGenerator::MIN_VELOCITY);
        REQUIRE(inlet.velocity <= InletGenerator::MAX_VELOCITY);
    }

    SECTION("Multiple Inlet Generation") {
        InletGenerator generator;
        auto inlets = generator.generateInlets(100);

        REQUIRE(inlets.size() == 100);

        // Check all inlets are within specified ranges
        for (const auto& inlet : inlets) {
            REQUIRE(inlet.throttle >= InletGenerator::MIN_THROTTLE);
            REQUIRE(inlet.throttle <= InletGenerator::MAX_THROTTLE);
            REQUIRE(inlet.velocity >= InletGenerator::MIN_VELOCITY);
            REQUIRE(inlet.velocity <= InletGenerator::MAX_VELOCITY);
        }
    }

    SECTION("Inlet Sorting") {
        InletGenerator generator;
        auto inlets = generator.generateInlets(100);

        // Verify sorting by checking each successive pair
        for (size_t i = 1; i < inlets.size(); ++i) {
            // Ensure throttle is non-decreasing
            REQUIRE(inlets[i-1].throttle <= inlets[i].throttle);

            // If throttles are essentially equal, check velocity
            if (std::abs(inlets[i-1].throttle - inlets[i].throttle) < 1e-9) {
                REQUIRE(inlets[i-1].velocity <= inlets[i].velocity);
            }
        }
    }

    SECTION("Reproducible Random Generation") {
        InletGenerator generator1(42);  // Seed for reproducibility
        InletGenerator generator2(42);  // Same seed

        auto inlets1 = generator1.generateInlets(10);
        auto inlets2 = generator2.generateInlets(10);

        // Should generate identical sequences with same seed
        REQUIRE(inlets1.size() == inlets2.size());
        for (size_t i = 0; i < inlets1.size(); ++i) {
            REQUIRE(inlets1[i].throttle == Approx(inlets2[i].throttle));
            REQUIRE(inlets1[i].velocity == Approx(inlets2[i].velocity));
        }
    }
}

