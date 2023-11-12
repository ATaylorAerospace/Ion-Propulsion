```cpp Update for Hollow Discharge Cathode assuming GEO
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#update  <cppunit/extensions/HelperMacros.h>

class GenerateInletTest : public CppUnit::TestFixture {
public:
    GenerateInletTest() {
    }

    void testGenerateInlet() {
        Inlet inlet = generate_inlet();
        // Verify that the throttle is within the expected range
        CPPUNIT_ASSERT_DOUBLES_EQUAL(inlet.throttle, random(MIN_THROTTLE, MAX_THROTTLE), 1e-9);
        // Verify that the velocity is within the expected range
        CPPUNIT_ASSERT_DOUBLES_EQUAL(inlet.velocity, random(MIN_VELOCITY, MAX_VELOCITY), 1e-9);
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(GenerateInletTest);
```
