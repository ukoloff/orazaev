#include <iostream>
#include <queue>
#include <vector>

#include <math.h>
#include <cassert>






class TAgent {
    int x;
    int y;

    public:
    TAgent(int x, int y)
        : x(x)
        , y(y) {
    }

    int X() const {
        return x;
    }

    int Y() const {
        return y;
    }

    bool operator== (const TAgent& rhs) const {
        return X() == rhs.X() && Y() == rhs.Y();
    }
};

typedef std::vector<TAgent> TAgentsVector;






class TEdge {
    TAgentsVector* agents;
    size_t firstAgent;
    size_t secondAgent;

    public:
    TEdge()
        : agents(NULL)
        , firstAgent(0)
        , secondAgent(0) {
    }

    TEdge(TAgentsVector* const agents, size_t firstAgent, size_t secondAgent)
        : agents(agents)
        , firstAgent(firstAgent)
        , secondAgent(secondAgent) {
    }

    const TAgent& GetFirstAgent() const {
        return (*agents)[firstAgent];
    }

    const TAgent& GetSecondAgent() const {
        return (*agents)[secondAgent];
    }

    size_t GetFirstAgentIndex() const {
        return firstAgent;
    }

    size_t GetSecondAgentIndex() const {
        return secondAgent;
    }

    double GetWeight() const {
        return sqrt(static_cast<double>(
            pow(GetFirstAgent().X() - GetSecondAgent().X(), 2) +
            pow(GetFirstAgent().Y() - GetSecondAgent().Y(), 2)));
    }

    bool operator< (const TEdge& rhs) const {
        // Use NOT for get minimum on top of std::priority_queue
        return !(GetWeight() < rhs.GetWeight());
    }
};


typedef std::priority_queue<TEdge> TEdgeHeap;







struct TDSUElement {
    size_t data;
    TDSUElement* pFather;
    size_t rank;

    explicit TDSUElement(const size_t& element)
        : data(element)
        , pFather(NULL)
        , rank(0) {
    }

    bool IsRoot() const {
        return pFather == NULL;
    }

    TDSUElement& Root() {
        if (IsRoot()) {
            return *this;
        }

        TDSUElement* node = pFather;

        while (!node->IsRoot()) {
            node = node->pFather;
        }

        TDSUElement* root = node;
        node = this;

        while (!node->IsRoot()) {
            TDSUElement* fatherNode = node->pFather;
            node->pFather = root;
            node = fatherNode;

            ++root->rank;
            --fatherNode->rank;
        }

        return *root;
    }

    bool operator== (const TDSUElement& rhs) {
        return data == rhs.data &&
               pFather == rhs.pFather &&
               rank == rhs.rank;
    }
};






class TAgentsDSU {
    size_t numberOfClasses;
    std::vector<TDSUElement> nodes;

    public:
    explicit TAgentsDSU(const TAgentsVector& agents)
        : numberOfClasses(agents.size())
        , nodes() {
        nodes.reserve(agents.size());

        for (int i = 0; i < agents.size(); ++i) {
            nodes.push_back(TDSUElement(i));
        }
    }

    bool Equivalent(size_t firstAgent, size_t secondAgent) {
        return nodes[firstAgent].Root() == nodes[secondAgent].Root();
    }

    void Unite(size_t firstAgent, size_t secondAgent) {
        if (Equivalent(firstAgent, secondAgent)) {
            return;
        }

        TDSUElement& firstRoot = nodes[firstAgent].Root();
        TDSUElement& secondRoot = nodes[secondAgent].Root();

        if (firstRoot.rank < secondRoot.rank) {
            firstRoot.pFather = &secondRoot;
            ++secondRoot.rank;
        } else {
            secondRoot.pFather = &firstRoot;
            ++firstRoot.rank;
        }

        --numberOfClasses;
    }

    size_t GetNumberOfClasses() const {
        return numberOfClasses;
    }
};







TEdgeHeap CreateEdgeHeap(TAgentsVector* const agents) {
    TEdgeHeap resultHeap;

    for (size_t firstAgent = 0;
         firstAgent < agents->size();
         ++firstAgent) {
        for (size_t secondAgent = 0;
             secondAgent < agents->size();
             ++secondAgent) {
            resultHeap.push(TEdge(agents, firstAgent, secondAgent));
        }
    }


    return resultHeap;
};






double FindRange(TAgentsVector* const agents) {
    TEdgeHeap edgeHeap = CreateEdgeHeap(agents);

    TAgentsDSU dsu(*agents);

    TEdge currentEdge;
    while (dsu.GetNumberOfClasses() > 1) {
        currentEdge = edgeHeap.top();
        edgeHeap.pop();

        dsu.Unite(currentEdge.GetFirstAgentIndex(),
                  currentEdge.GetSecondAgentIndex());
    }

    return currentEdge.GetWeight();
};






void test_TEdge() {
    TAgentsVector agents;
    agents.push_back(TAgent(4, 5));
    agents.push_back(TAgent(1, 1));
    agents.push_back(TAgent(0, 3));
    agents.push_back(TAgent(0, 9));

    TEdge foo(&agents, 0, 1);
    assert(foo.GetWeight() == 5.0);

    TEdge bar(&agents, 2, 3);
    assert(bar.GetWeight() == 6.0);

    assert(foo.GetWeight() < bar.GetWeight());

    TEdgeHeap edgeHeap;
    edgeHeap.push(bar);
    edgeHeap.push(foo);
    edgeHeap.push(bar);

    assert(edgeHeap.top().GetFirstAgent() == foo.GetFirstAgent());
    assert(edgeHeap.top().GetSecondAgent() == foo.GetSecondAgent());
}






void test_TAgentsDSU() {
    TAgentsVector agents;
    agents.push_back(TAgent(4, 5));
    agents.push_back(TAgent(1, 1));
    agents.push_back(TAgent(0, 3));
    agents.push_back(TAgent(0, 9));

    TAgentsDSU dsu(agents);

    assert(!dsu.Equivalent(0, 1));
    assert(dsu.Equivalent(0, 0));

    assert(dsu.GetNumberOfClasses() == 4);

    dsu.Unite(0, 3);
    assert(dsu.Equivalent(0, 3));
    assert(dsu.Equivalent(3, 0));
    assert(!dsu.Equivalent(3, 1));
    assert(!dsu.Equivalent(3, 2));
    assert(!dsu.Equivalent(1, 0));
    assert(dsu.Equivalent(0, 0));
    assert(dsu.Equivalent(3, 3));

    assert(dsu.GetNumberOfClasses() == 3);
}





int main() {
    test_TEdge();
    test_TAgentsDSU();

    return 0;
}
