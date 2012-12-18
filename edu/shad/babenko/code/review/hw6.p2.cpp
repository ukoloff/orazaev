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


double CalculateDistanceBetweenAgents(
    const TAgent& first,
    const TAgent& second
) {
    return sqrt(static_cast<double>(
        pow(first.GetX() - second.GetX(), 2) +
        pow(first.GetY() - second.GetY(), 2)));
}




class TEdge {
    size_t firstAgentIndex;
    size_t secondAgentIndex;
    double weight;

    public:
    TEdge()
        : firstAgentIndex(0)
        , secondAgentIndex(0)
        , weight(0.0) {
    }

    TEdge(size_t firstAgentIndex, size_t secondAgentIndex, double weight)
        : firstAgentIndex(firstAgentIndex)
        , secondAgentIndex(secondAgentIndex)
        , weight(weight) {
    }

    size_t GetFirstAgentIndex() const {
        return firstAgentIndex;
    }

    size_t GetSecondAgentIndex() const {
        return secondAgentIndex;
    }

    double GetWeight() const {
        return weight;
    }

    bool operator< (const TEdge& rhs) const {
        // Use 'NOT' for get minimum on top of std::priority_queue
        return !(GetWeight() < rhs.GetWeight());
    }
};


typedef std::priority_queue<TEdge> TEdgeHeap;







class TAgentsDSU {
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

        bool operator== (const TDSUElement& rhs) {
            return data == rhs.data &&
                   pFather == rhs.pFather &&
                   rank == rhs.rank;
        }
    };

    TDSUElement* Root(TDSUElement* element) {
        if (element->IsRoot()) {
            return element;
        }

        TDSUElement* node = element->pFather;

        while (!node->IsRoot()) {
            node = node->pFather;
        }

        TDSUElement* root = node;
        node = element;

        while (!node->IsRoot()) {
            TDSUElement* fatherNode = node->pFather;
            node->pFather = root;
            node = fatherNode;

            ++root->rank;
            --fatherNode->rank;
        }

        return root;
    }

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
        return Root(&nodes[firstAgent]) == Root(&nodes[secondAgent]);
    }

    void Unite(size_t firstAgent, size_t secondAgent) {
        if (Equivalent(firstAgent, secondAgent)) {
            return;
        }

        TDSUElement* firstRoot = Root(&nodes[firstAgent]);
        TDSUElement* secondRoot = Root(&nodes[secondAgent]);

        if (firstRoot->rank < secondRoot->rank) {
            firstRoot->pFather = secondRoot;
            ++secondRoot->rank;
        } else {
            secondRoot->pFather = firstRoot;
            ++firstRoot->rank;
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
                double edgeWeight = CalculateDistanceBetweenAgents(
                        agents->at(firstAgent),
                        agents->at(secondAgent));

                resultHeap.push(TEdge(firstAgent, secondAgent, edgeWeight));
            }
        }
    }


    return resultHeap;
};





double FindOptimalRangeForAgentsRadio(TAgentsVector* agents) {
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
    printf("%.10f", FindOptimalRangeForAgentsRadio(&agents));

    return 0;
}
