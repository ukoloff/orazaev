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
    int xCoordinate;
    int yCoordinate;

    public:
    TAgent(int xCoordinate, int yCoordinate)
        : xCoordinate(xCoordinate)
        , yCoordinate(yCoordinate) {
    }

    int GetX() const {
        return xCoordinate;
    }

    int GetY() const {
        return yCoordinate;
    }

    bool operator== (const TAgent& rhs) const {
        return GetX() == rhs.GetX() && GetY() == rhs.GetY();
    }
};

typedef std::vector<TAgent> TAgentsVector;






std::ostream& operator<< (std::ostream& out, const TAgent& agent) {
    out << "[" << agent.GetX() << ", " << agent.GetY() << "]";
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
            pow(GetFirstAgent().GetX() - GetSecondAgent().GetX(), 2) +
            pow(GetFirstAgent().GetY() - GetSecondAgent().GetY(), 2)));
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
        size_t xCoordinate, yCoordinate;
        in >> xCoordinate >> yCoordinate;
        result.push_back(TAgent(xCoordinate, yCoordinate));
    }

    return result;
}








int main() {
    TAgentsVector agents = ReadAgents(std::cin);
    printf("%.10f", FindRange(&agents));

    return 0;
}
