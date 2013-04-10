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

    inline void SetSuffixLink(size_t link) { suffixLink = link; }

    /**
        @brief Get child node index in nodes vector.
        @return child node index if has and TNode::NONE otherwise.
    */
    size_t GetChild(char edgeFirstChar) const;
    static const size_t NONE = static_cast<size_t>(-1);

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
    assert(children.find(edgeFirstChar) == children.end());
    children[edgeFirstChar] = childIndex;
}


int main() {

    return 0;
}
