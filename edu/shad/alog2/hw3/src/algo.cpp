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

    inline void SetSuffixLink(const TNodePtr& link) { suffixLink = link; }
    inline void SetEdgeLabelEnd(size_t newEnd) { edgeLabelEnd = newEnd; }
    inline void SetEdgeLabelBegin(size_t newBegin) {
        edgeLabelBegin = newBegin;
    }

    inline bool IsLeaf() const { return children.size() == 0; }
    TNodePtr GetChild(char edgeFirstChar) const;
    void AddChild(char edgeFirstChar, const TNodePtr& child);
    inline bool HasChild(char edgeFirstChar) const {
        return GetChild(edgeFirstChar) == 0;
    }

    char GetEdgeChar(const std::string& text,
            char firstEdgeChar,
            size_t position);

    char GetCharOnEdge(const std::string& text, size_t position) const;

    static TNodePtr PassTheEdges(TNodePtr node,
            size_t fullDepth,
            const std::string& text,
            size_t i);

public:
    static const size_t NONE = static_cast<size_t>(-1);

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

char TNode::GetCharOnEdge(const std::string& text, size_t position) const {
    return text[GetEdgeLabelBegin() + position];
}

char TNode::GetEdgeChar(const std::string& text,
        char firstEdgeChar,
        size_t position)
{
    if (!HasChild(firstEdgeChar)) {
        return 0;
    }

    return text[GetChild(firstEdgeChar)->GetEdgeLabelBegin() + position];
}

static TNodePtr TNode::PassTheEdges(TNodePtr node,
        size_t fullDepth,
        const std::string& text,
        size_t i)
{
    size_t pos = i - (fullDepth - node.GetDepth());
    if (!HasChild(text[pos])) {
        return node;
    }

    for (TNodePtr child = node->GetChild(text[pos]);
         child->GetDepth() < fullDepth();
         child = child->GetChild(text[pos]))
    {
        node = child;
        pos = i - (fullDepth - node.GetDepth());

        if (!HasChild(text[pos])) {
            return node;
        }
    }

    return node;
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

    TNodePtr node = root;
    size_t length = 0;
    char character = 0;

    for (size_t i = 0; i < text.size(); ++i) {
        /* Add new child to current node */
        if (character == 0 && !node->HasChild(text[i])) {
            for (TNodePtr n = node;
                !n->HasChild(text[i]);
                n = n->GetSuffixLink())
            {
                n->AddChild(text[i], suffixTree.CreateLeaf(i, root));
            }
            continue;
        }

        if (character == 0 && node->HasChild(text[i])) {
            ++length;
            character = text[i];
            continue;
        }

        TNodePtr child = node->GetChild(character);

        /* Is edge was ended? Switch node to child if true. */
        if (!child->IsLeaf() &&
            child->GetEdgeSize() >= length)
        {
            node = child;
            length = 0;
            character = 0;
            i -= 1;
            continue;
        }

        char nextEdgeSymbol = child.GetCharOnEdge(text, length);

        /* Is nextEdgeSymbol equal to text[i]? Add node if false. */
        if (nextEdgeSymbol == text[i]) {
            ++length;
            continue;
        }

        /// Need tot use full depth instead of length on current edge.
        /// That hint simplify code.
        while (child.GetCharOnEdge(text, length) != text[i]) {
            /* Create new node on edge */
            TNodePtr newNode =
                    suffixTree.CreateNode(child->GetEdgeLabelBegin(),
                    child->GetEdgeLabelBegin() + length,
                    node->GetDepth() + length, root);

            child->SetEdgeLabelBegin(newNode->GetEdgeLabelEnd());
            newNode->AddChild(nextEdgeSymbol,
                    node->GetChild(nextEdgeSymbol));
            newNode->AddChild(text[i], suffixTree.CreateLeaf(i, root));

            node->AddChild(newNode->GetCharOnEdge(text, 0), newNode);

            TNodePtr nodeByLink = node->GetSuffixLink();
            --length;
            TNodePtr nodeByLink = TNode::PassTheEdges(nodeByLink,
                    node.GetDepth() + length, text, i);
            i - nodeByLink.GetDepth();

            length = node.GetDepth() + length - nodeByLink->GetDepth();
            character = text[i - length];

            character = text[i - (newNode->GetDepth() -
                    nodeByLink->GetDepth() - 1)];
            character = [i - (newNode->GetDepth - 1)]
            newNode->SetSuffixLink(nodeByLink);
        }
    }

    return result;
}


int main() {

    return 0;
}
