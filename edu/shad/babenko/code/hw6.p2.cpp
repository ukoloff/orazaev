/*
 *  Copyright (c) 2012 Aman Orazaev
 *
 *  Homework 6, Problem 2
 *
 *  Agents radio.
 *
 */
#include <math.h>
#include <cassert>
#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <queue>
#include <vector>






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






std::ostream& operator<< (std::ostream& out, const TAgent& agent) {
    out << "[" << agent.X() << ", " << agent.Y() << "]";
}







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

    TEdge(TAgentsVector* agents, size_t firstAgent, size_t secondAgent)
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
        // Use 'NOT' for get minimum on top of std::priority_queue
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
    explicit TAgentsDSU(size_t agentsSize)
        : numberOfClasses(agentsSize)
        , nodes() {
        nodes.reserve(agentsSize);

        for (int i = 0; i < agentsSize; ++i) {
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







TEdgeHeap CreateEdgeHeap(TAgentsVector* agents) {
    TEdgeHeap resultHeap;

    for (size_t firstAgent = 0;
         firstAgent < agents->size();
         ++firstAgent) {
        for (size_t secondAgent = 0;
             secondAgent < agents->size();
             ++secondAgent) {
            if (firstAgent != secondAgent) {
                resultHeap.push(TEdge(agents, firstAgent, secondAgent));
            }
        }
    }


    return resultHeap;
};





double FindRange(TAgentsVector* agents) {
    if (agents->size() < 2) {
        return 0.0;
    }

    TEdgeHeap edgeHeap = CreateEdgeHeap(agents);

    TAgentsDSU dsu(agents->size());

    TEdge currentEdge;
    while (dsu.GetNumberOfClasses() > 1) {
        currentEdge = edgeHeap.top();
        edgeHeap.pop();

        dsu.Unite(currentEdge.GetFirstAgentIndex(),
                  currentEdge.GetSecondAgentIndex());
    }

    return currentEdge.GetWeight();
};







TAgentsVector ReadAgents(std::istream& in) {
    size_t size;
    in >> size;

    TAgentsVector result;
    result.reserve(size);

    for (int i = 0; i < size; ++i) {
        size_t x, y;
        in >> x >> y;
        result.push_back(TAgent(x, y));
    }

    return result;
}







/////////////////////////// Testing here //////////////////////////////




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

    TAgentsDSU dsu(agents.size());

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






/////////////////////////// Stress testing here ///////////////////////////




template <typename T>
void printElements(T begin, T end) {
    while (begin != end) {
        std::cout << *begin++ << " ";
    }
    std::cout << std::endl;
}





class TAgentNode;

typedef std::vector<TAgentNode> TAgentNodeVector;



class TAgentNode : public TAgent {
    std::vector<TAgentNode*> agentsInRange;

    bool wasHere;

    public:
    TAgentNode(int x, int y)
        : TAgent(x, y)
        , agentsInRange()
        , wasHere(false) {
    }

    void AddAgentInRange(TAgentNode* const pAgentNode) {
        agentsInRange.push_back(pAgentNode);
    }

    std::vector<TAgentNode*>::iterator begin() {
        return agentsInRange.begin();
    }

    std::vector<TAgentNode*>::iterator end() {
        return agentsInRange.end();
    }

    std::vector<TAgentNode*>::const_iterator begin() const {
        return agentsInRange.begin();
    }

    std::vector<TAgentNode*>::const_iterator end() const {
        return agentsInRange.end();
    }

    size_t GetNumberOfConnectedAgents() const {
        return agentsInRange.size();
    }

    bool WasHere() const {
        return wasHere;
    }

    void SetWasHere(bool newWasHere) {
        wasHere = newWasHere;
    }

    typedef std::vector<TAgentNode*>::iterator iterator;
    typedef std::vector<TAgentNode*>::const_iterator const_iterator;
};







double CalculateRange(const TAgentNode& first, const TAgentNode& second) {
    return sqrt(static_cast<double>(
        pow(first.X() - second.X(), 2) +
        pow(first.Y() - second.Y(), 2)));
}







TAgentNodeVector CreateAgentNodeVector(
    const TAgentsVector& agents,
    double range
) {
    TAgentNodeVector result;

    for (TAgentsVector::const_iterator cit = agents.begin();
         cit != agents.end();
         ++cit) {
        result.push_back(TAgentNode(cit->X(), cit->Y()));
    }


    for (size_t current = 0;
         current < result.size();
         ++current) {
        for (size_t other = 0;
             other < result.size();
             ++other) {
            if (other == current) {
                continue;
            }

            if (CalculateRange(result[current], result[other]) <= range) {
                result[current].AddAgentInRange(&result[other]);
            }
        }
    }

    return result;
}





std::ostream& operator<< (std::ostream& out, const TAgentNode& agent) {
    out << "[(" << agent.X() << ", " << agent.Y() << "), (";

    for (TAgentNode::const_iterator cit = agent.begin();
         cit != agent.end();
         ++cit) {
        std::cout << "<" << (*cit)->X() << ", " << (*cit)->Y() << ">";
        if (cit + 1 != agent.end()) {
            std::cout << " ";
        }
    }
    std::cout << ")]";
}





size_t NumberOfConnectedNodes(
    TAgentNode* const agent
) {
    size_t connected = 0;
    if (agent->WasHere()) {
        return connected;
    }

    agent->SetWasHere(true);
    ++connected;

    for (TAgentNode::const_iterator cit = agent->begin();
         cit != agent->end();
         ++cit) {
        if (!(*cit)->WasHere()) {
            connected += NumberOfConnectedNodes(*cit);
        }
    }

    return connected;
}






bool IsAssociatedGraph(TAgentNodeVector& graph) {
    if (graph.size() == 0) {
        return true;
    }

    return NumberOfConnectedNodes(&graph[0]) == graph.size();
}






TAgent CreateRandomAgent() {
    int x = (rand() % 1000000) * pow(-1, rand() % 2);
    int y = (rand() % 1000000) * pow(-1, rand() % 2);
    return TAgent(x, y);
}






TAgentsVector CreateRandomAgentsVector(
    size_t maxSize,
    size_t minSize = 10) {
    size_t size = rand() % (maxSize - minSize) + minSize;

    TAgentsVector agents;

    for (int i = 0; i < size; ++i) {
        agents.push_back(CreateRandomAgent());
    }

    return agents;
}





class TTimer {
    clock_t startClocks;
    clock_t endClocks;

    public:
    TTimer()
        : startClocks(0)
        , endClocks(0) {
    }

    void Start() {
        startClocks = clock();
    }

    void Stop() {
        endClocks = clock();
    }

    double GetSeconds() const {
        return static_cast<double>(
            endClocks - startClocks) / CLOCKS_PER_SEC;
    }

    clock_t GetClocks() const {
        return endClocks - startClocks;
    }
};







const double accuracy = 0.00000001;

void test_Stress(size_t numberOfTests) {
    TTimer timer;
    for (size_t n = 0; n < numberOfTests; ++n) {
        std::cout << "Test #" << n + 1 << std::endl;

        timer.Start();
        TAgentsVector agents = CreateRandomAgentsVector(1001, 1000);
        // std::cout << "    Agents: ";
        // printElements(agents.begin(), agents.end());
        std::cout << "    Agents size: " << agents.size() << std::endl;

        double ans = FindRange(&agents);
        timer.Stop();
        std::cout << "    Founded range: ";
        printf("%.10f\n", ans);
        std::cout << "    TIME: " << timer.GetSeconds() << std::endl;
        assert(timer.GetSeconds() < 2.5);

        TAgentNodeVector associated =
                    CreateAgentNodeVector(agents, ans);

        // std::cout << "    Associated: ";
        // printElements(associated.begin(), associated.end());

        assert(IsAssociatedGraph(associated));

        if (ans - accuracy > 0) {
            TAgentNodeVector notAssociated =
                        CreateAgentNodeVector(agents, ans - accuracy);
            assert(!IsAssociatedGraph(notAssociated));
        }
    }
}






int main() {
    // std::srand(360);
    // test_TEdge();
    // test_TAgentsDSU();
    // test_Stress(100);


    TAgentsVector agents = ReadAgents(std::cin);
    printf("%.10f", FindRange(&agents));

    return 0;
}
