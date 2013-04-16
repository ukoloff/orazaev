#include <cassert>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

class TNode;
typedef std::shared_ptr<TNode> TNodePtr;

class TNode {
public:
    TNode()
        : suffixLink(0)
        , edgeLabelBegin(0)
        , edgeLabelEnd(0)
        , depth(0)
    { }

    TNode(size_t edgeLabelBegin,
        size_t edgeLabelEnd,
        size_t depth,
        const TNodePtr& suffixLink)
        : suffixLink(suffixLink)
        , edgeLabelBegin(edgeLabelBegin)
        , edgeLabelEnd(edgeLabelEnd)
        , depth(depth)
    { }

public:
    inline TNodePtr GetSuffixLink() const { return suffixLink; }
    inline size_t GetEdgeLabelBegin() const { return edgeLabelBegin; }
    inline size_t GetEdgeLabelEnd() const { return edgeLabelEnd; }
    inline size_t GetDepth() const { return depth; }
    inline size_t GetEdgeSize() const {
        return edgeLabelEnd - edgeLabelBegin;
    }

    inline bool IsLeaf() const { return children.size() == 0; }

    inline void SetSuffixLink(const TNodePtr& link) { suffixLink = link; }
    inline void SetEdgeLabelEnd(size_t newEnd) { edgeLabelEnd = newEnd; }
    inline void SetEdgeLabelBegin(size_t newBegin) {
        edgeLabelBegin = newBegin;
    }

    /**
        @brief Get child node index in nodes vector.
        @return child node index if has and TNode::NONE otherwise.
    */
    TNodePtr GetChild(char edgeFirstChar) const;
    static const size_t NONE = static_cast<size_t>(-1);

    inline bool HasChild(char edgeFirstChar) const {
        return GetChild(edgeFirstChar) == 0;
    }

    void AddChild(char edgeFirstChar, const TNodePtr& child);

private:
    TNodePtr suffixLink;
    size_t edgeLabelBegin;
    size_t edgeLabelEnd;
    size_t depth;
    std::unordered_map<char, TNodePtr> children;
};

TNodePtr TNode::GetChild(char edgeFirstChar) const {
    auto iter = children.find(edgeFirstChar);
    if (iter == children.end()) {
        return 0;
    }

    return iter->second;
}

void TNode::AddChild(char edgeFirstChar, const TNodePtr& child) {
    children[edgeFirstChar] = child;
}



class TSuffixTree {
public:
    TSuffixTree()
        : root(CreateNode())
    {
        root->SetSuffixLink(root);
    }

    TNodePtr GetRoot() {
        return root;
    }

public:
    static TNodePtr CreateNode() {
        return TNodePtr(new TNode());
    }

    static TNodePtr CreateNode(size_t edgeLabelBegin,
            size_t edgeLabelEnd,
            size_t depth,
            const TNodePtr& suffixLink )
    {
        return TNodePtr(new TNode(edgeLabelBegin, edgeLabelEnd,
                depth, suffixLink));
    }

    static TNodePtr CreateLeaf(size_t edgeLabelBegin,
            const TNodePtr& suffixLink) {
        return TNodePtr(new TNode(edgeLabelBegin, TNode::NONE,
                TNode::NONE, suffixLink));
    }

private:
    std::vector<TNode> nodes;
    TNodePtr root;
};




std::vector<size_t> Calc(std::string&& text) {
    text += '$';
    std::vector<size_t> result(text.size(), 0);

    TSuffixTree suffixTree;
    TNodePtr root = suffixTree.GetRoot();

    TNodePtr currentNode = root;
    size_t currentLength = 0;
    char currentSymbol = 0;

    for (size_t i = 0; i < text.size(); ++i) {
        /* Add new child to current node */
        if (currentLength == 0 && !currentNode->HasChild(text[i])) {
            for (TNodePtr node = currentNode;
                !node->HasChild(text[i]);
                node = node->GetSuffixLink())
            {
                node->AddChild(text[i], suffixTree.CreateLeaf(i, root));
            }
            continue;
        }

        if (currentLength == 0 && currentNode->HasChild(text[i])) {
            ++currentLength;
            currentSymbol = text[i];
        }

        TNodePtr currentEdge = currentNode->GetChild(currentSymbol);

        /* Is edge was ended? Switch currentNodeIndex if true. */
        if (!currentEdge->IsLeaf() &&
            currentEdge->GetEdgeSize() >= currentLength)
        {
            currentNode = currentNode->GetChild(text[i]);
            currentLength = 0;
            currentSymbol = 0;
            i -= 1;
            continue;
        }

        char nextEdgeSymbol =
            text[currentEdge->GetEdgeLabelBegin() + currentLength];

        /* Is nextEdgeSymbol equal to text[i]? Add node if false. */
        if (nextEdgeSymbol == text[i]) {
            ++currentLength;
            continue;
        }

        /* assert(nextEdgeSymbol != text[i]) */
        TNodePtr currentEdgeIndex = currentNode->GetChild(currentSymbol);
        TNodePtr newNode =
                suffixTree.CreateNode(currentEdge->GetEdgeLabelBegin(),
                currentEdge->GetEdgeLabelBegin() + currentLength,
                currentNode->GetDepth() + currentLength, root);

        currentEdge->SetEdgeLabelBegin(newNode->GetEdgeLabelEnd());
        newNode->AddChild(nextEdgeSymbol,
                currentNode->GetChild(nextEdgeSymbol));

        currentNode->AddChild(text[newNode->GetEdgeLabelBegin()],
                newNode);

        /* add leaf here, and suffix jump */

    }

    return result;
}


int main() {

    return 0;
}
