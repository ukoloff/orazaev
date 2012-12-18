#ifndef _HW6_TEST_CODE_H_
#define _HW6_TEST_CODE_H_


/////////////////////////// Testing here //////////////////////////////




void test_TEdge() {
    TAgentsVector agents;
    agents.push_back(TAgent(4, 5));
    agents.push_back(TAgent(1, 1));
    agents.push_back(TAgent(0, 3));
    agents.push_back(TAgent(0, 9));

    TEdge foo(0, 1, CalculateDistanceBetweenAgents(agents[0], agents[1]));
    assert(foo.GetWeight() == 5.0);

    TEdge bar(2, 3, CalculateDistanceBetweenAgents(agents[2], agents[3]));
    assert(bar.GetWeight() == 6.0);

    assert(foo.GetWeight() < bar.GetWeight());

    TEdgeHeap edgeHeap;
    edgeHeap.push(bar);
    edgeHeap.push(foo);
    edgeHeap.push(bar);

    assert(edgeHeap.top().GetFirstAgentIndex() == foo.GetFirstAgentIndex());
    assert(edgeHeap.top().GetSecondAgentIndex() == foo.GetSecondAgentIndex());
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
    TAgentNode(int xCoordinate, int yCoordinate)
        : TAgent(xCoordinate, yCoordinate)
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
        pow(first.GetX() - second.GetX(), 2) +
        pow(first.GetY() - second.GetY(), 2)));
}







TAgentNodeVector CreateAgentNodeVector(
    const TAgentsVector& agents,
    double range
) {
    TAgentNodeVector result;

    for (TAgentsVector::const_iterator cit = agents.begin();
         cit != agents.end();
         ++cit) {
        result.push_back(TAgentNode(cit->GetX(), cit->GetY()));
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
    out << "[(" << agent.GetX() << ", " << agent.GetY() << "), (";

    for (TAgentNode::const_iterator cit = agent.begin();
         cit != agent.end();
         ++cit) {
        std::cout << "<" << (*cit)->GetX() << ", " << (*cit)->GetY() << ">";
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
    int xCoordinate = (rand() % 1000000) * pow(-1, rand() % 2);
    int yCoordinate = (rand() % 1000000) * pow(-1, rand() % 2);
    return TAgent(xCoordinate, yCoordinate);
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
    for (size_t test = 0; test < numberOfTests; ++test) {
        std::cout << "Test #" << test + 1 << std::endl;

        timer.Start();
        TAgentsVector agents = CreateRandomAgentsVector(1001, 1000);
        // std::cout << "    Agents: ";
        // printElements(agents.begin(), agents.end());
        std::cout << "    Agents size: " << agents.size() << std::endl;

        double ans = FindOptimalRangeForAgentsRadio(&agents);
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








#endif // _HW6_TEST_CODE_H_
