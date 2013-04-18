#pragma once
#include <cppunit/MiniCppUnit.hxx>

typedef std::shared_ptr<TSuffixTree> TTreePtr;

class TTestNode : public TestFixture<TTestNode> {
public:
    TEST_FIXTURE(TTestNode) {
        /** List of all test cases to run. */
        TEST_CASE(TestConstruction);
    }

    virtual void setUp() {
        /** Run this code before each test. */
        text = "abcbxabx";
        tree = TTreePtr(new TSuffixTree(text));
    }

    virtual void tearDown() {
        /** Run this code after each test. */
    }

    void TestConstruction() {
        tree = TTreePtr(new TSuffixTree(""));
        ASSERT_EQUALS(true, tree->GetRoot()->IsLeaf());
    }

private:
    TTreePtr tree;
    std::string text;
};



/** Register test fixture to run. */
REGISTER_FIXTURE(TTestNode);

int RunTests() {
    return TestFixtureFactory::theInstance().runTests() ? 0 : 1;
}
