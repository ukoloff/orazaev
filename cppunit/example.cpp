#include <MiniCppUnit.hxx>

class TTestExample : public TestFixture<TTestExample> {
public:
    TEST_FIXTURE(TTestExample) {
        /** List of all test cases to run. */
        TEST_CASE(TestAsserts);
    }

    virtual void setUp() {
        /** Run this code before each test. */
        pNumber = new int(42);
    }

    virtual void tearDown() {
        /** Run this code after each test. */
        delete pNumber;
    }

    void TestAsserts() {
        /** Call abort() if 42 != *pNumber */
        ASSERT_EQUALS(42, *pNumber);

        /** Call abort() and print msg if 42 != *pNumber */
        ASSERT_MESSAGE(42 == *pNumber, "*pNumber is not equal to 42");

        /** Call abort() if *pNumber is not in [42.0001 - 0.1; 42.0001 + 0.1] */
        ASSERT_EQUALS_EPSILON(42.0001, (double) *pNumber, 0.1);

        if (pNumber == 0) {
            /** abort() and print why */
            FAIL("pNumber is points to 0!");
        }
    }

private:
    int* pNumber; 
};



/** Register test fixture to run. */
REGISTER_FIXTURE(TTestExample);

int main() {
    return TestFixtureFactory::theInstance().runTests() ? 0 : 1;
}
