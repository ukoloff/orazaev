/**
    Copyright 2013 Aman Orazaev
*/
#include <cassert>

#include <iostream>
#include <vector>
#include <string>
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
        , ancestor(ancestor)
    { }

    TNode()
        : depth(0)
        , suffixLink(0)
        , ancestor(0)
    { }

public:
    inline void SetSuffixLink(const TNodePtr& link) { suffixLink = link; }
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
    inline void SetBegin(size_t value) { begin = value; }

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
    TReminder(const TNodePtr& node, size_t length)
        : node(node)
        , length(length)
    { }

    TNodePtr node;
    size_t length;
};



/**
    @brief Text, useful functions for all nodes and edges
*/
class TSuffixTree {
public:
    TSuffixTree()
        : text("")
        , root(ConstructNode(0, 0, 0))
    {
        root->SetSuffixLink(root);
    }

public:
    inline TNodePtr GetRoot() const          { return root; }
    void SetText(const std::string& newText) { text = newText; }

    std::vector<size_t> ConstructTreeAndCalcRepeatings(const std::string& text);

    /**
        @brief apply string text[begin:end] to suffix tree node
        @return TNodePtr to last node on the string way
    */
    TNodePtr ApplyString(TNodePtr node, size_t begin, size_t end) {
        assert(begin <= end);

        if (begin >= end) {
            return node;
        }

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

public:
    TEdgePtr ConstructEdge(size_t begin, const TNodePtr& ancestor) const {
        return TEdgePtr(new TEdge(begin, text, ancestor));
        return std::make_shared<TEdge>(begin, text, ancestor);
    }

    TEdgePtr ConstructEdge(size_t begin, size_t end, const TNodePtr& node) const {
        return TEdgePtr(new TEdge(begin, end, text, node));
        return std::make_shared<TEdge>(begin, end, text, node);
    }

    static TNodePtr ConstructNode(size_t depth,
            const TNodePtr& suffixLink,
            const TNodePtr& ancestor)
    {
        return TNodePtr(new TNode(depth, suffixLink, ancestor));
        return std::make_shared<TNode>(depth, suffixLink, ancestor);
    }

public:
    static const char SENTINEL = '$';

protected:
    /**
        @return TNodePtr for new node on sliced edge.
    */
    TNodePtr SplitEdge(const TReminder& reminder, size_t textPosition) {
        TEdgePtr edge = reminder.node->GetEdge(
                text[textPosition - reminder.length]);

        assert(edge->GetChar(reminder.length) != text[textPosition]);

        TNodePtr newNode = ConstructNode(reminder.node->GetDepth() +
                reminder.length, 0, reminder.node);
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
        @brief Do suffix link jump from newNode using ancestor suffix link.
        @return TNodePtr to suffix jump destenation node.
    */
    TNodePtr DoSuffixLinkJump(TReminder * const reminder,
            const TNodePtr newNode,
            size_t position)
    {
        if (reminder->node == GetRoot()) {
            reminder->node = ApplyString(reminder->node->GetSuffixLink(),
                    position - (newNode->GetDepth() - 1), position);
        } else {
            reminder->node = ApplyString(reminder->node->GetSuffixLink(),
                    position - (newNode->GetDepth() -
                    reminder->node->GetDepth()), position);
        }

        /// Update reminder values
        reminder->length =
                (newNode->GetDepth() - 1) - reminder->node->GetDepth();
        TEdgePtr nextEdge =
                reminder->node->GetEdge(text[position - reminder->length]);

        /// Boundary case: length is equal to nextEdge size
        if (nextEdge && reminder->length == nextEdge->GetSize()) {
            reminder->node = nextEdge->GetNode();
            reminder->length = 0;
        }

        return reminder->node;
    }

private:
    std::string text;
    TNodePtr root;
};



std::vector<size_t> TSuffixTree::ConstructTreeAndCalcRepeatings(
        const std::string& input_text)
{
    SetText(input_text + TSuffixTree::SENTINEL);
    std::vector<size_t> result(text.size(), 0);

    root = ConstructNode(0, 0, 0);
    root->SetSuffixLink(root);

    TReminder reminder(root, 0);
    TNodePtr suffixLinkNeed = 0;

    /// Main cycle
    for (size_t pos = 0; pos < text.size(); ++pos) {
        TEdgePtr edge = reminder.node->GetEdge(text[pos - reminder.length]);

        /// Add new edge if hasnt.
        if (edge == 0) {
            TEdgePtr newEdge = ConstructEdge(pos, reminder.node);
            reminder.node->SetEdge(text[pos], newEdge);
            result[pos - reminder.node->GetDepth()] =
                    reminder.node->GetDepth();

            if (reminder.node != GetRoot()) {
                assert(reminder.node->GetAncestor() != 0);
                reminder.length += reminder.node->GetDepth() - 1;
                --pos;
                reminder.node = reminder.node->GetSuffixLink();
                reminder.length -= reminder.node->GetDepth();
            }
            continue;
        }

        /// Update reminder if we can pass the edge.
        if (edge->GetChar(reminder.length) == text[pos]) {
            if (edge->GetSize() <= ++reminder.length) {
                reminder.node = edge->GetNode();
                reminder.length = 0;
                assert(reminder.node->GetAncestor() != 0);
            }
            continue;
        }

        /// Split edge and pass through suffix link
        TNodePtr newNode = SplitEdge(reminder, pos);
        result[pos - newNode->GetDepth()] = newNode->GetDepth();
        assert(newNode->GetAncestor() != 0);

        /// Set suffixLink for previous split-node
        if (suffixLinkNeed != 0) {
            suffixLinkNeed->SetSuffixLink(newNode);
            assert(suffixLinkNeed->GetDepth() == newNode->GetDepth() + 1 ||
                   suffixLinkNeed == GetRoot());
        }
        suffixLinkNeed = newNode;

        DoSuffixLinkJump(&reminder, newNode, pos--);
        if (reminder.length == 0) {
            suffixLinkNeed->SetSuffixLink(reminder.node);
            assert(suffixLinkNeed->GetDepth() == reminder.node->GetDepth() +
                   1 || suffixLinkNeed == GetRoot());
            suffixLinkNeed = 0;
        }
    }

    assert(reminder.length == 0);

    result.pop_back();
    return result;
}



#ifdef UNIT_TESTING
#   include "hw3.p4.testing.h"
#endif /// UNIT_TESTING



int main() {
#ifdef UNIT_TESTING
    return RunTests();
#endif /// UNIT_TESTING

    std::string input;
    std::cin >> input;

    std::vector<size_t> result =
            TSuffixTree().ConstructTreeAndCalcRepeatings(input);

    for (auto e : result) {
        std::cout << e << "\n";
    }

    return 0;
}
