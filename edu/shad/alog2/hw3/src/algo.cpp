#include <iostream>
#include <vector>
#include <cassert>
#include <unordered_map>

class TNode {
public:
    TNode()
        : suffixLink(0)
        , edgeLabelBegin(0)
        , edgeLabelEnd(0)
        , depth(0)
    { }

    TNode(size_t edgeLabelBegin, size_t edgeLabelEnd, size_t depth)
        : suffixLink(0)
        , edgeLabelBegin(edgeLabelBegin)
        , edgeLabelEnd(edgeLabelEnd)
        , depth(depth)
    { }

    inline size_t GetSuffixLink() const { return suffixLink; }
    inline size_t GetEdgeLabelBegin() const { return edgeLabelBegin; }
    inline size_t GetEdgeLabelEnd() const { return edgeLabelEnd; }
    inline size_t GetDepth() const { return depth; }
    inline size_t GetEdgeSize() const {
        return edgeLabelEnd - edgeLabelBegin;
    }

    inline bool IsLeaf() const { return GetDepth() == NONE; }

    inline void SetSuffixLink(size_t link) { suffixLink = link; }
    inline void SetEdgeLabelEnd(size_t newEnd) { edgeLabelEnd = newEnd; }
    inline void SetEdgeLabelBegin(size_t newBegin) {
        edgeLabelBebin = newBegin;
    }

    /**
        @brief Get child node index in nodes vector.
        @return child node index if has and TNode::NONE otherwise.
    */
    size_t GetChild(char edgeFirstChar) const;
    static const size_t NONE = static_cast<size_t>(-1);

    inline bool HasChild(char edgeFirstChar) const {
        return GetChild(edgeFirstChar) != TNode::NONE;
    }

    void AddChild(char edgeFirstChar, size_t childIndex);

private:
    size_t suffixLink;
    size_t edgeLabelBegin;
    size_t edgeLabelEnd;
    size_t depth;
    std::unordered_map<char, size_t> children;
};

size_t TNode::GetChild(char edgeFirstChar) const {
    auto iter = children.find(edgeFirstChar);
    if (iter == children.end()) {
        return TNode::NONE;
    }

    return iter->second;
}

void TNode::AddChild(char edgeFirstChar, size_t childIndex) {
    children[edgeFirstChar] = childIndex;
}



class TTree {
public:
    TTree() { }

    TNode& operator[] (size_t index) { return nodes[index]; }
    const TNode& operator[] (size_t index) const { return nodes[index]; }

    size_t CreateNode() {
        nodes.push_back(TNode());
        return nodes.size() - 1;
    }

    size_t CreateNode(size_t edgeLabelBegin,
            size_t edgeLabelEnd,
            size_t depth)
    {
        nodes.push_back(TNode(edgeLabelBegin, edgeLabelEnd, depth));
        return nodes.size() - 1;
    }

    size_t CreateLeaf(size_t edgeLabelBegin) {
        nodes.push_back(TNode(edgeLabelBegin, TNode::NONE, TNode::NONE));
        return nodes.size() - 1;
    }
private:
    std::vector<TNode> nodes;
};




std::vector<size_t> Calc(std::string&& text) {
    text += '$';
    std::vector<size_t> result(text.size(), 0);

    TTree suffixTree;
    size_t root = suffixTree.CreateNode();

    size_t currentNodeIndex = root;
    size_t currentLength = 0;
    char currentSymbol = 0;

    for (int i = 0; i < text.size(); ++i) {
        TNode& currentNode = suffixTree[currentNodeIndex];

        /* Add new child to current node */
        if (currentLength == 0 && !currentNode.HasChild(text[i])) {
            for (TNode& node = currentNode;
                !node.HasChild(text[i]);
                node = suffixTree[node.GetSuffixLink()])
            {
                node.AddChild(text[i], suffixTree.CreateLeaf(i));
            }
            continue;
        }

        if (currentLength == 0 && currentNode.HasChild(text[i])) {
            ++currentLength;
            currentSymbol = text[i];
        }

        TNode& currentEdge = suffixTree[currentNode.GetChild(currentSymbol)]

        /* Is edge was ended? Switch currentNodeIndex if true. */
        if (!IsLeaf(currentEdge) &&
            currentEdge.GetEdgeSize() >= currentLength)
        {
            currentNodeIndex = currentNode.GetChild(text[i]);
            currentLength = 0;
            currentSymbol = 0;
            i -= 1;
            continue;
        }

        char nextEdgeSymbol =
            text[currentEdge.GetEdgeLabelBegin() + currentLength];

        /* Is nextEdgeSymbol equal to text[i]? Add node if false. */
        if (nextEdgeSymbol == text[i]) {
            ++currentLength;
            continue;
        }

        /* assert(nextEdgeSymbol != text[i]) */
        size_t currentEdgeIndex = currentNode.GetChild(currentSymbol);
        size_t newNodeIndex =
                suffixTree.CreateNode(currentEdge.GetEdgeLabelBegin,
                currentEdge.GetEdgeLabelBegin() + currentLength,
                currentNode.GetDepth() + currentLength);
        TNode& newNode = suffixTree[newNodeIndex];

        currentEdge.SetEdgeLabelBegin(newNode.GetEdgeLabelEnd());
        newNode.AddChild(nextEdgeSymbol, currentNode.GetChild(nextEdgeSymbol))

        currentNode.AddChild(text[newNode.GetEdgeLabelBegin()],
                newNodeIndex);

        /* add leaf here, and suffix jump */

    }

    return result;
}


int main() {

    return 0;
}
