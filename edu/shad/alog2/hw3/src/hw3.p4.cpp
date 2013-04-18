#include <cassert>
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
    TNode(size_t depth, const TNodePtr& suffixLink, const TNodePtr& ancestor)
        : depth(depth)
        , suffixLink(suffixLink)
    { }

    TNode()
        : depth(0)
        , suffixLink(0)
        , ancestor(0)
    { }

public:
    inline void SetSuffixLink(const TNodePtr& link) { suffixLink = link; }
    // inline void SetDepth(size_t newDepth)           { depth = newDepth; }
    inline void SetAncestor(const TNodePtr& node) { ancestor = node; }
    inline void SetEdge(char firstChar, const TEdgePtr& edge) {
        edges[firstChar] = edge;
    }

public:
    inline bool IsLeaf() const            { return !edges.size(); }
    inline TNodePtr GetSuffixLink() const { return suffixLink; }
    inline TNodePtr GetAncestor() const   { return ancestor; }
    inline size_t GetDepth() const        { return depth; }

    TEdgePtr GetEdge(char character) const {
        return edges.find(character) != edges.end() ?
                edges.find(character)->second : 0;
    }

private:
    std::unordered_map<char, TEdgePtr> edges;
    size_t depth;
    TNodePtr suffixLink;
    TNodePtr ancestor;
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

    TEdge(size_t begin, const std::string& text, const TNodePtr& ancestor)
        : edgeNode(ancestor)
        , begin(begin)
        , end(ENDLESS)
        , text(text)
    { }

public:
    inline bool IsEndless() const  { return GetEnd() == ENDLESS; }
    inline size_t GetEnd() const   { return end; }
    inline size_t GetBegin() const { return begin; }

    inline size_t GetSize() const  {
        return IsEndless() ? ENDLESS : GetEnd() - GetBegin();
    }
    inline TNodePtr GetNode() const {
        return IsEndless() ? 0 : edgeNode;
    }
    inline TNodePtr GetAncestor() const {
        return IsEndless() ? edgeNode : edgeNode->GetAncestor();
    }
    inline char GetChar(size_t index) const {
        return index < GetSize() ? text[GetBegin() + index] : 0;
    }

public:
    inline void SetBegin(size_t value)       { begin = value; }
    // inline void SetEnd(size_t value)         { end = value; }
    // inline void SetNode(const TNodePtr& ptr) { edgeNode = ptr; }
    inline void SetAncestor(const TNodePtr& ancestor) {
        if (IsEndless()) {
            edgeNode = ancestor;
        } else {
            edgeNode->SetAncestor(ancestor);
        }
    }

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
    TSuffixTree(const std::string& text)
        : text(text)
        , root(ConstructNode(0, 0, 0))
    {
        root->SetSuffixLink(root);
    }

public:
    inline TNodePtr GetRoot() const { return root; }

    /**
        @return TNodePtr for new node on sliced edge.
    */
    TNodePtr SplitEdge(const TReminder& reminder, size_t textPosition) {
        TEdgePtr edge = reminder.node->GetEdge(
                text[textPosition - reminder.length]);
        assert(edge->GetChar(reminder.length) == text[textPosition]);

        TNodePtr newNode = ConstructNode(reminder.node->GetDepth() +
                reminder.length, root, reminder.node);
        TEdgePtr newEdge = ConstructEdge(edge->GetBegin(),
                edge->GetBegin() + reminder.length, newNode);

        /// Reduce old edge
        edge->SetBegin(edge->GetBegin() + reminder.length);
        edge->SetAncestor(newNode);
        /// Attach old edge to new node
        newNode->SetEdge(edge->GetChar(0), edge);
        /// Rebind edge for ancestor
        reminder.node->SetEdge(newEdge->GetChar(0), newEdge);

        /// Add endless edge to newNode
        newNode->SetEdge(text[textPosition],
                ConstructEdge(textPosition, reminder.node));

        return newNode;
    }

    /**
        @brief apply string text[begin:end] to suffix tree node
        @return TNodePtr to last node on the string way
    */
    TNodePtr ApplyString(TNodePtr node, size_t begin, size_t end) {
        assert(begin <= end);
        do {
            TEdgePtr edge = node->GetEdge(text[begin]);
            begin = begin + edge->GetSize();
            if (edge->IsEndless() || begin >= end) {
                break;
            }

            node = edge->GetNode();
        } while (begin < end);

        return node;
    }

    /**
        @brief Do suffix link jump from newNode using ancestor suffix link.
        @return TNodePtr to suffix jump destenation node.
    */
    TNodePtr DoSuffixLinkJump(TReminder * const reminder,
            const TNodePtr newNode,
            size_t position)
    {
        reminder->node = ApplyString(reminder->node->GetSuffixLink(),
                position - (newNode->GetDepth() - 1), position);

        /// Update reminder values
        reminder->length =
                (newNode->GetDepth() - 1) - reminder->node->GetDepth();
        TEdgePtr nextEdge =
                reminder->node->GetEdge(text[position - reminder->length]);
        reminder->character =
                reminder->length == 0 ? 0 : nextEdge->GetChar(0);

        /// Boundary case: length is equal to nextEdge size
        if (reminder->length == nextEdge->GetSize()) {
            reminder->node = nextEdge->GetNode();
            reminder->length = 0;
            reminder->character = 0;
        }

        return reminder->node;
    }
public:
    TEdgePtr ConstructEdge(size_t begin, const TNodePtr& ancestor) {
        return TEdgePtr(new TEdge(begin, text, ancestor));
    }
    TEdgePtr ConstructEdge(size_t begin, size_t end, const TNodePtr& node) {
        return TEdgePtr(new TEdge(begin, end, text, node));
    }
    static TNodePtr ConstructNode(size_t depth,
            const TNodePtr& suffixLink,
            const TNodePtr& ancestor)
    {
        return TNodePtr(new TNode(depth, suffixLink, ancestor));
    }

public:
    static const char SENTINEL = '$';

private:
    const std::string& text;
    TNodePtr root;
};


std::vector<size_t> CalcSolution(std::string& text) {
    text += TSuffixTree::SENTINEL;
    std::vector<size_t> result(text.size(), 0);

    TSuffixTree tree(text);
    TNodePtr root = tree.GetRoot();

    TReminder reminder(root, 0, 0);

    /// Main cycle
    for (size_t i = 0; i < text.size(); ++i) {
        TEdgePtr edge = reminder.node->GetEdge(text[i - reminder.length]);

        /// Add new edge if hasnt.
        if (reminder.character == 0 && edge == 0) {
            TEdgePtr newEdge = tree.ConstructEdge(i, reminder.node);
            reminder.node->SetEdge(text[i], newEdge);
            reminder.character = text[i];

            /// Suffix jump
            reminder.node = reminder.node->GetSuffixLink();
            --i;
            continue;
        }

        /// Update reminder if we can pass the edge.
        if (edge->GetChar(reminder.length) == text[i]) {
            if (edge->GetSize() >= ++reminder.length) {
                reminder.node = edge->GetNode();
                reminder.length = 0;
                reminder.character = 0;
            }
            continue;
        }

        /**
            ASSERT: we can't pass the way on edge and we should split
            the edge
        */

        /// Split edge and pass through suffix link
        TNodePtr newNode = tree.SplitEdge(reminder, i);

        /// Set suffixLink for newNode and update reminder
        newNode->SetSuffixLink(tree.DoSuffixLinkJump(&reminder, newNode, i--));
    }


    result.pop_back();
    return result;
}

#include "hw3.p4.testing.h"

int main() {
    return RunTests();
}
