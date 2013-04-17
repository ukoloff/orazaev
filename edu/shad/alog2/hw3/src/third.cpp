#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>


class TNode;
class TEdge;
typedef std::shared_ptr<TNode> TNodePtr;
typedef std::shared_ptr<TEdge> TEdgePtr;


class TNode {
public:
    TNode(size_t depth, const TNodePtr& suffixLink /* args */)
        : depth(depth)
        , suffixLink(suffixLink)
    { }

    TNode()
        : depth(0)
        , suffixLink(0)
    { }

public:
    inline bool IsLeaf() const;
    TNodePtr SuffixLink() const;

    TEdgePtr Edge(char character) const;

private:
    std::unordered_map<char, TEdgePtr> edges;
    size_t depth;
    TNodePtr suffixLink;
};


class TEdge {
public:
    TEdge(size_t begin,
            size_t end,
            const std::string& text,
            const TNodePtr& edgeNode)
        : edgeNode(edgeNode)
        , begin(begin)
        , end(end)
        , text(text)
    { }

    TEdge(size_t begin,
            size_t end,
            const std::string& text)
        : edgeNode(0)
        , begin(begin)
        , end(end)
        , text(text)
    { }

    TEdge(size_t begin, const std::string& text)
        : edgeNode(0)
        , begin(begin)
        , end(ENDLESS)
        , text(text)
    { }

public:
    size_t Begin() const;
    size_t End() const;

    size_t Size() const;
    TNodePtr Node() const;

    char GetChar(size_t index) const;

public:
    void SetBegin(size_t);
    void SetEnd(size_t);
    void SetNode(const TNodePtr&);

public:
    static const size_t ENDLESS = static_cast<size_t>(-1);

private:
    TNodePtr edgeNode;
    size_t begin;
    size_t end;
    const std::string& text;
};


struct TReminder {
    TReminder(const TNodePtr& node,
            char character,
            size_t length)
        : node(node)
        , character(character)
        , length(length)
    { }

    TNodePtr node;
    char character;
    size_t length;
};


/**
    @brief Text, useful functions for all nodes and edges
*/
class TSuffixTree {
public:
    TSuffixTree(const std::string& text /* args */)
        : text(text)
        , root(ConstructNode(0, 0))
    { }

    TNodePtr GetRoot() const;

    /** @returns new TNodePtr for node on edge. */
    TNodePtr SplitEdge(TReminder* reminder, size_t textPosition);

public:
    TEdgePtr ConstructEdge(size_t begin);
    TEdgePtr ConstructEdge(size_t begin, size_t end, const TNodePtr& node);
    static TNodePtr ConstructNode(size_t depth, const TNodePtr& suffixLink);

private:
    const std::string& text;
    TNodePtr root;
};


std::vector<size_t> CalcSolution(std::string& text) {
    text += '$';
    std::vector<size_t> result(text.size(), 0);
    
    TSuffixTree tree(text);
    TNodePtr root = tree.GetRoot();
    
    // TODO: Fill reminder


    /* -------------- BUILD TREE AND CALCULATE HERE ------------------ */
    for (size_t i = 0; i < text.size(); ++i) {
        // TODO: character == 0:
            // TODO: Add node if hasn't, switch reminder.character, continue
        

        // TODO: Can we pass the way on edge?
            // TODO: If so, ++length,
            // TODO: Is edge ended?
                // TODO: If so, change reminder.node;
            // TODO: continue;

        // ASSERT: we can't pass the way on edge and we should slice
        // the edge

        // TODO: Create new edge, new node
        // TODO: Split edge 
        // TODO: Pass through suffix link;
        // TODO: Pass the way on nodeByLink and add new suffix link
        // TODO: Caclulate new reminder values (node, character, length)
    }

    
    result.pop_back();
    return result;
}
