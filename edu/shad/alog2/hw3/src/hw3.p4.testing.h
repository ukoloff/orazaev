#pragma once
#include <cppunit/MiniCppUnit.hxx>
#include <util/timer/timer.h>

typedef std::shared_ptr<TSuffixTree> TTreePtr;

std::ostream& operator<< (std::ostream& out, const std::vector<size_t>& v) {
    out << "{ ";
    for (auto e : v) {
        out << e << " ";
    }
    out << "}";

    return out;
}

class TTestNode : public TestFixture<TTestNode> {
public:
    TEST_FIXTURE(TTestNode) {
        /** List of all test cases to run. */
        TEST_CASE(TestTrivialOperations);
        TEST_CASE(TestApplyString);
        TEST_CASE(TestTrivialConstruction);
        TEST_CASE(TestMinimalConstruction);
        TEST_CASE(TestMinimalSplittedConstruction);
        TEST_CASE(TestSplittedConstruction);
        TEST_CASE(TestRandomStringsConstruction);
        //TEST_CASE(TestTiming);
        //TEST_CASE(TestTimingWorstCase);
        TEST_CASE(TestTrivialRepeatings);
        TEST_CASE(TestStressRepeatings);
    }

    virtual void setUp() {
        /** Run this code before each test. */
        text = "abcbxabx";
        tree = TTreePtr(new TSuffixTree());
        tree->SetText(text);

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
        tree = TTreePtr(new TSuffixTree());
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

    void TestTrivialConstruction() {
        tree->ConstructTreeAndCalcSolution("");
        ASSERT_EQUALS(false, tree->GetRoot()->IsLeaf());

        TEdgePtr edge = tree->GetRoot()->GetEdge('$');
        ASSERT_MESSAGE(edge != 0, "root->GetEdge('$') equals to 0.");
        ASSERT_EQUALS(true, edge->IsEndless());
        ASSERT_EQUALS('$', edge->GetChar(0));
    }

    void TestMinimalConstruction() {
        tree->ConstructTreeAndCalcSolution("abcd");
        ASSERT_EQUALS(false, tree->GetRoot()->IsLeaf());

        for (auto c : std::string("abcd$")) {
            TEdgePtr edge = tree->GetRoot()->GetEdge(c);
            ASSERT_MESSAGE(edge != 0, (std::string("root->GetEdge('") +
                        c + "') equals to 0.").c_str());
            ASSERT_EQUALS(true, edge->IsEndless());
            ASSERT_EQUALS(c, edge->GetChar(0));
        }
    }

    void TestMinimalSplittedConstruction() {
        tree->ConstructTreeAndCalcSolution("aab");
        ASSERT_EQUALS(false, tree->GetRoot()->IsLeaf());

        for (auto c : std::string("b$")) {
            TEdgePtr edge = tree->GetRoot()->GetEdge(c);
            ASSERT_MESSAGE(edge != 0, (std::string("root->GetEdge('") + c +
                        "') equals to 0.").c_str());
            ASSERT_EQUALS(true, edge->IsEndless());
            ASSERT_EQUALS(c, edge->GetChar(0));
        }

        TEdgePtr edge = tree->GetRoot()->GetEdge('a');
        ASSERT_MESSAGE(edge != 0, "root->GetEdge(SENTINEL) equals to 0.");
        ASSERT_EQUALS(false, edge->IsEndless());
        ASSERT_EQUALS('a', edge->GetChar(0));

        TNodePtr node = edge->GetNode();
        ASSERT_MESSAGE(tree->GetRoot() != node, "Split-node is equal to root");
        ASSERT_EQUALS(size_t(1), edge->GetNode()->GetDepth());

        ASSERT_EQUALS(node, tree->ApplyString(tree->GetRoot(), 1, 3));
        ASSERT_EQUALS(node, tree->ApplyString(tree->GetRoot(), 0, 3));

        for (auto c : std::string("ab")) {
            TEdgePtr edge = node->GetEdge(c);
            ASSERT_MESSAGE(edge != 0, (std::string("Split-node->GetEdge('")
                        + c + "') equals to 0.").c_str());
            ASSERT_EQUALS(true, edge->IsEndless());
            ASSERT_EQUALS(c, edge->GetChar(0));
        }
    }

    void TestSplittedConstruction() {
        ApplyAllSuffixes("ababbbab");
        ApplyAllSuffixes("aeeadbedbbbdbc");
        ApplyAllSuffixes("abxbyabyabz");
        ApplyAllSuffixes("adbcbxadbxadbz");
        ApplyAllSuffixes("aaax");
        ApplyAllSuffixes("abxabyabz");
        ApplyAllSuffixes("abxabyabzabwabh");
        ApplyAllSuffixes("abxabyabzabwabhtrutritrotrabutrabw");
        ApplyAllSuffixes("abwabhtrutrabtrabw");
        ApplyAllSuffixes("abxabycdecdmaby");
        ApplyAllSuffixes("abxabycaby");
        ApplyAllSuffixes("");
        ApplyAllSuffixes("abbabb");
    }

    void TestRandomStringsConstruction() {
        srand(13);
        for (int i = 0; i < 200; ++i) {
            ApplyAllSuffixes(GetRandomString(200, 26));
        }
    }

    void TestTiming() {
        srand(13);
        double time = 1;
        TTimer timer;

        for (int i = 0; i < 10; ++i) {
            std::string s = GetRandomString(100000);

            timer.Start();
            TSuffixTree().ConstructTreeAndCalcSolution(s);
            timer.Stop();

            ASSERT_MESSAGE(timer.GetSeconds() < time, "time limit exceeded");
        }
    }

    void TestTimingWorstCase() {
        std::string s = "";
        for (size_t i = 0; i < 100000; ++i) {
            s += 'a';
        }

        TTimer timer;

        timer.Start();
        TSuffixTree().ConstructTreeAndCalcSolution(s);
        timer.Stop();

        std::cout << timer.GetSeconds() << std::endl;
        ASSERT_MESSAGE(timer.GetSeconds() < 1, "time limit exceeded");
    }

    void TestTrivialRepeatings() {
        const size_t SIZE = 2;
        std::vector<size_t> expected[SIZE] = {
            {0, 4, 3, 2, 1},
            {0, 0, 3, 2, 1, 2, 1}
        };
        std::string input[SIZE] = {
            "aaaaa",
            "ababaab"
        };
        for (size_t i = 0; i < SIZE; ++i) {
            ASSERT_EQUALS(expected[i],
                    TSuffixTree().ConstructTreeAndCalcSolution(input[i]));
        }
    }

    void TestStressRepeatings() {
        for (size_t i = 0; i < 2000; ++i) {
            std::string input = GetRandomString(30, 26);
            ASSERT_EQUALS(CalcRepeatings(input),
                    TSuffixTree().ConstructTreeAndCalcSolution(input));
        }
    }

    std::vector<size_t> CalcRepeatings(const std::string input) {
        std::vector<size_t> result(input.size(), 0);
        for (size_t i = 0; i < input.size(); ++i) {
            for (size_t j = 0; j < i; ++j) {
                size_t k = 0;
                while (input[i + k] == input[j + k]) {
                    ++k;
                    if (i + k == input.size())
                        break;
                }
                result[i] = result[i] < k ? k : result[i];
            }
        }

        return result;
    }

    void ApplyAllSuffixes(const std::string& text) {
        tree->ConstructTreeAndCalcSolution(text);
#ifdef VERBOSE_OUTPUT
        std::cout << "TREE for text: " << text << std::endl;
        tree->Print(tree->GetRoot());
#endif

        for (size_t i = 0; i < text.size(); ++i) {
            TNodePtr node = tree->ApplyString(tree->GetRoot(), i, text.size());
            TEdgePtr edge = node->GetEdge(text[i + node->GetDepth()]);
            if (!edge->IsEndless()) {
                ASSERT_EQUALS(text.size() - i - node->GetDepth(), edge->GetSize());
            }
        }
    }


    std::string GetRandomString(int maxLength, int maxAlphabetSize) {
        int length = rand() % maxLength + 1;
        int aSize = rand() % maxAlphabetSize + 1;

        std::string result = "";

        for (int i = 0; i < length; ++i) {
            result += 'a' + (rand() % aSize);
        }

        return result;
    }

    std::string GetRandomString(int length) {
        std::string result = "";

        for (int i = 0; i < length; ++i) {
            result += 'a' + (rand() % 25);
        }

        return result;
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
