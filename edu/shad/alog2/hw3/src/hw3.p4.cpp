#include <iostream>
#include <vector>
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

    void SetSuffixLink(size_t link);

private:
    size_t suffixLink;
    size_t edgeLabelBegin;
    size_t edgeLabelEnd;
    size_t depth;
    std::unordered_map<char, size_t> children;
};


int main() {

    return 0;
}
