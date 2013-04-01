#pragma once
#include <cstdlib>
#include <cppunit/MiniCppUnit.hxx>

template <typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T> vec) {
    PrintElements(vec.begin(), vec.end(), &out);
    return out;
}

std::string GetRandomString(size_t maxLength) {
    size_t size = rand() % maxLength + 1;
    std::string result(size, 0);

    for (std::string::iterator it = result.begin();
         it != result.end();
         ++it)
    {
        char randChar = 'a' + rand() % 26;
        *it = randChar;
    }

    return result;
}

std::vector<size_t> TrivialGetPrefixFunction(const std::string& s) {
    std::vector<size_t> result = std::vector<size_t>(s.size(), 0);

    for (size_t i = 1; i < s.size(); ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (s.substr(0, j + 1) == s.substr(i - j, j + 1)) {
                result[i] = j + 1;
            }
        }
    }

    return result;
}

class TTestHw1P1 : public TestFixture<TTestHw1P1> {
public:
    TEST_FIXTURE(TTestHw1P1) {
        TEST_CASE(TestTrivial);
        TEST_CASE(TestStress);
    }

    virtual void setUp() {
        std::srand(42);
    }

    void TestTrivial() {
        ASSERT_EQUALS(TrivialGetPrefixFunction("aaaa"), GetPrefixFunction("aaaa"));
    }

    void TestStress() {
        for (int i = 0; i < 50000; ++i) {
            std::string string = GetRandomString(100);
            std::cout << "Test No " << i << ": " << string << "\n";
            ASSERT_EQUALS(
                TrivialGetPrefixFunction(string),
                GetPrefixFunction(string));
        }
    }

};

REGISTER_FIXTURE(TTestHw1P1);

int RunTests() {
    return TestFixtureFactory::theInstance().runTests() ? 0 : 1;
}
