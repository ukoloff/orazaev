#pragma once
#include <cppunit/MiniCppUnit.hxx>

typedef std::shared_ptr<TSuffixTree> TTreePtr;

class TTestNode : public TestFixture<TTestNode> {
public:
    TEST_FIXTURE(TTestNode) {
        /** List of all test cases to run. */
        TEST_CASE(TestTrivialOperations);
        TEST_CASE(TestApplyString);
    }

    virtual void setUp() {
        /** Run this code before each test. */
        text = "abcbxabx";
        tree = TTreePtr(new TSuffixTree(text));

        TNodePtr node = tree->ConstructNode(2, 0, tree->GetRoot());
        /// root -> ab
        TEdgePtr edge = tree->ConstructEdge(0, 2, node);
        tree->GetRoot()->SetEdge(edge->GetChar(0), edge);

        /// root -> ab -> c...
        edge = tree->ConstructEdge(2, node);
        node->SetEdge(edge->GetChar(0), edge);

        /// root -> ab -> x...
        edge = tree->ConstructEdge(7, node);
        node->SetEdge(edge->GetChar(0), edge);
    }

    virtual void tearDown() {
        /** Run this code after each test. */
    }

    void TestTrivialOperations() {
        tree = TTreePtr(new TSuffixTree(""));
        ASSERT_EQUALS(true, tree->GetRoot()->IsLeaf());

        tree->GetRoot()->SetEdge(text[0],
                tree->ConstructEdge(0, tree->GetRoot()));
        ASSERT_EQUALS(false, tree->GetRoot()->IsLeaf());

        TEdgePtr edge = tree->GetRoot()->GetEdge(text[0]);
        ASSERT_EQUALS(static_cast<size_t>(-1), edge->GetEnd());
        ASSERT_EQUALS(static_cast<size_t>(-1), edge->GetSize());
        ASSERT_EQUALS(true, edge->IsEndless());
        ASSERT_EQUALS(tree->GetRoot(), edge->GetAncestor());
    }

    void TestApplyString() {
        TNodePtr node = tree->ApplyString(tree->GetRoot(), 0, 1);
        ASSERT_EQUALS(tree->GetRoot(), node);

        /// Apply ""
        node = tree->ApplyString(tree->GetRoot(), 0, 0);
        ASSERT_EQUALS(tree->GetRoot(), node);

        /// Apply "a"
        node = tree->ApplyString(tree->GetRoot(), 0, 1);
        ASSERT_EQUALS(tree->GetRoot(), node);

        /// Apply "ab", boundary case: expected root
        node = tree->ApplyString(tree->GetRoot(), 0, 2);
        ASSERT_EQUALS(tree->GetRoot(), node);

        /// Apply "abc"
        TNodePtr expectedNode = tree->GetRoot()->GetEdge(text[0])->GetNode();
        node = tree->ApplyString(tree->GetRoot(), 0, 3);
        ASSERT_EQUALS(expectedNode, node);

        /// Apply "abcde"
        node = tree->ApplyString(tree->GetRoot(), 0, 5);
        ASSERT_EQUALS(expectedNode, node);
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
