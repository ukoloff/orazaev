/**
    Copyright 2013 Aman Orazaev
*/
#include <cassert>

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <unordered_map>



class TSuffixTreeNode;
class TSuffixTreeEdge;
typedef std::shared_ptr<TSuffixTreeNode> TSuffixTreeNodePtr;
typedef std::shared_ptr<TSuffixTreeEdge> TSuffixTreeEdgePtr;



class TSuffixTreeNode {
public:
    TSuffixTreeNode(size_t depth, const TSuffixTreeNodePtr& suffixLink, const TSuffixTreeNodePtr& ancestor)
        : depthInChar(depth)
        , suffixLink(suffixLink)
        , ancestor(ancestor)
    { }

    TSuffixTreeNode()
        : depthInChar(0)
        , suffixLink(0)
        , ancestor(0)
    { }

public:
    void SetSuffixLink(const TSuffixTreeNodePtr& link) { suffixLink = link; }
    void SetAncestor(const TSuffixTreeNodePtr& node) { ancestor = node; }
    void SetEdge(char firstChar, const TSuffixTreeEdgePtr& edge) {
        edges[firstChar] = edge;
    }

public:
    bool IsLeaf() const            { return edges.empty(); }
    TSuffixTreeNodePtr GetSuffixLink() const { return suffixLink; }
    TSuffixTreeNodePtr GetAncestor() const   { return ancestor; }
    size_t GetDepth() const        { return depthInChar; }

    bool HasEdge(char character) const {
        return edges.find(character) != edges.end();
    }

    TSuffixTreeEdgePtr GetEdge(char character) const {
        return HasEdge(character) ?
                edges.find(character)->second : 0;
    }

private:
    std::unordered_map<char, TSuffixTreeEdgePtr> edges;
    size_t depthInChar;
    TSuffixTreeNodePtr suffixLink;
    TSuffixTreeNodePtr ancestor;
};



class TSuffixTreeEdge {
public:
    TSuffixTreeEdge(size_t beginIndex,
            size_t endIndex,
            const std::string& text,
            const TSuffixTreeNodePtr& targetNode)
        : targetNode(targetNode)
        , beginIndex(beginIndex)
        , endIndex(endIndex)
        , text(text)
    { }

    TSuffixTreeEdge(size_t beginIndex, const std::string& text, const TSuffixTreeNodePtr& ancestor)
        : targetNode(ancestor)
        , beginIndex(beginIndex)
        , endIndex(NO_END)
        , text(text)
    { }

public:
    inline bool IsEndless() const  { return GetEnd() == NO_END; }
    inline size_t GetEnd() const   { return endIndex; }
    inline size_t GetBegin() const { return beginIndex; }

    inline size_t GetLength() const  {
        return IsEndless() ? NO_END : GetEnd() - GetBegin();
    }

    inline TSuffixTreeNodePtr GetTargetNode() const {
        return IsEndless() ? 0 : targetNode;
    }

    inline TSuffixTreeNodePtr GetSourceNode() const {
        return IsEndless() ? targetNode : targetNode->GetAncestor();
    }

    inline char GetChar(size_t index) const {
        assert(index < GetLength());
        return text[GetBegin() + index];
    }

public:
    inline void SetBegin(size_t value) { beginIndex = value; }

    inline void SetAncestor(const TSuffixTreeNodePtr& ancestor) {
        if (IsEndless()) {
            targetNode = ancestor;
        } else {
            targetNode->SetAncestor(ancestor);
        }
    }

public:
    static const size_t NO_END = static_cast<size_t>(-1);

private:
    TSuffixTreeNodePtr targetNode;
    size_t beginIndex;
    size_t endIndex;
    const std::string& text;
};



struct TReminder {
    TReminder(const TSuffixTreeNodePtr& node, size_t length)
        : node(node)
        , length(length)
    { }

    TSuffixTreeNodePtr node;
    size_t length;
};



/**
    @brief Suffix tree class.
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
    inline TSuffixTreeNodePtr GetRoot() const          { return root; }

    std::vector<size_t> InitTreeAndCalculateRepeatings(const std::string& text);

    /**
        @brief apply string text[begin:end] to suffix tree node
        @return TSuffixTreeNodePtr to last node on the string way
    */
    TSuffixTreeNodePtr ApplyString(TSuffixTreeNodePtr node, size_t begin, size_t end) {
        assert(begin <= end);

        if (begin >= end) {
            return node;
        }

        do {
            TSuffixTreeEdgePtr edge = node->GetEdge(text[begin]);
            begin = begin + edge->GetLength();
            if (edge->IsEndless() || begin >= end) {
                break;
            }

            node = edge->GetTargetNode();
        } while (begin < end);

        return node;
    }

public:
    static const char SENTINEL = '$';

private:
    /**
        @return TSuffixTreeNodePtr for new node on sliced edge.
    */
    TSuffixTreeNodePtr SplitEdge(const TReminder& reminder, size_t textPosition) {
        TSuffixTreeEdgePtr edge = reminder.node->GetEdge(
                text[textPosition - reminder.length]);

        assert(edge->GetChar(reminder.length) != text[textPosition]);

        TSuffixTreeNodePtr newNode = ConstructNode(reminder.node->GetDepth() +
                reminder.length, 0, reminder.node);
        TSuffixTreeEdgePtr newEdge = ConstructEdge(edge->GetBegin(),
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
        @return TSuffixTreeNodePtr to suffix jump destenation node.
    */
    TSuffixTreeNodePtr DoSuffixLinkJump(TReminder * const reminder,
            const TSuffixTreeNodePtr newNode,
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
        TSuffixTreeEdgePtr nextEdge =
                reminder->node->GetEdge(text[position - reminder->length]);

        /// Boundary case: length is equal to nextEdge size
        if (nextEdge && reminder->length == nextEdge->GetLength()) {
            reminder->node = nextEdge->GetTargetNode();
            reminder->length = 0;
        }

        return reminder->node;
    }

    void AddNewEdgeToReminderNode(TReminder * const reminder,
            size_t * const position,
            std::vector<size_t> * const repeatings)
    {
        TSuffixTreeEdgePtr newEdge = ConstructEdge(*position, reminder->node);
        reminder->node->SetEdge(text[*position], newEdge);
        repeatings->at(*position - reminder->node->GetDepth()) =
            reminder->node->GetDepth();

        if (reminder->node != GetRoot()) {
            assert(reminder->node->GetAncestor() != 0);
            reminder->length += reminder->node->GetDepth() - 1;
            --*position;
            reminder->node = reminder->node->GetSuffixLink();
            reminder->length -= reminder->node->GetDepth();
        }
    }

private:
    TSuffixTreeEdgePtr ConstructEdge(size_t begin, const TSuffixTreeNodePtr& ancestor) const {
        return TSuffixTreeEdgePtr(new TSuffixTreeEdge(begin, text, ancestor));
        return std::make_shared<TSuffixTreeEdge>(begin, text, ancestor);
    }

    TSuffixTreeEdgePtr ConstructEdge(size_t begin, size_t end, const TSuffixTreeNodePtr& node) const {
        return TSuffixTreeEdgePtr(new TSuffixTreeEdge(begin, end, text, node));
        return std::make_shared<TSuffixTreeEdge>(begin, end, text, node);
    }

    static TSuffixTreeNodePtr ConstructNode(size_t depth,
            const TSuffixTreeNodePtr& suffixLink,
            const TSuffixTreeNodePtr& ancestor)
    {
        return TSuffixTreeNodePtr(new TSuffixTreeNode(depth, suffixLink, ancestor));
        return std::make_shared<TSuffixTreeNode>(depth, suffixLink, ancestor);
    }

    void SetText(const std::string& newText) { text = newText; }

private:
    std::string text;
    TSuffixTreeNodePtr root;

#ifdef UNIT_TESTING
    friend class TTestNode;
#endif
};



std::vector<size_t> TSuffixTree::InitTreeAndCalculateRepeatings(
        const std::string& input_text)
{
    SetText(input_text + TSuffixTree::SENTINEL);
    std::vector<size_t> result(text.size(), 0);

    root = ConstructNode(0, 0, 0);
    root->SetSuffixLink(root);

    TReminder reminder(root, 0);
    TSuffixTreeNodePtr suffixLinkNeed = 0;

    /// Main cycle
    for (size_t pos = 0; pos < text.size(); ++pos) {
        TSuffixTreeEdgePtr edge = reminder.node->GetEdge(text[pos - reminder.length]);

        /// Add new edge if hasnt.
        if (edge == 0) {
            AddNewEdgeToReminderNode(&reminder, &pos, &result);
            continue;
        }

        /// Update reminder if we can pass the edge.
        if (edge->GetChar(reminder.length) == text[pos]) {
            ++reminder.length;
            if (edge->GetLength() <= reminder.length) {
                reminder.node = edge->GetTargetNode();
                reminder.length = 0;
                assert(reminder.node->GetAncestor() != 0);
            }
            continue;
        }

        /// Split edge and pass through suffix link
        TSuffixTreeNodePtr newNode = SplitEdge(reminder, pos);
        result[pos - newNode->GetDepth()] = newNode->GetDepth();

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
            assert(suffixLinkNeed->GetDepth() == reminder.node->GetDepth()
                   + 1 || suffixLinkNeed == GetRoot());
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
            TSuffixTree().InitTreeAndCalculateRepeatings(input);

    for (auto e : result) {
        std::cout << e << "\n";
    }

    return 0;
}
