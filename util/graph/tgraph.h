#ifndef TGRAPH_H
#define TGRAPH_H
#include<vector>
#include<memory>

class TNodeList;


// TNode description
template <class T>
class TNode {
    T node_data;
    TNodeList<T> node_edges;    

    TNode(const TNode& n) { /*noncopyable*/ }
    TNode<T>& operator=(const TNode& n) { /*noncopyable*/ }
public:
    TNode() {
    }

    TNode(const T& t)
        : node_data(t)
        , node_edges() {
    }

    virtual ~TNode() {
    }

    T& data() { 
        return node_data; 
    }

    TNodeList& edges() { 
        return node_edges; 
    }
};


// TNodeList description
template <class T>
class TNodeList {
    std::vector<TNode<T>*> node_list;
    
public:
    TNodeList() {
    }

    TNodeList(const TNodeList<T>& n) {
        for(int x = 0; x < n.size(); x++)
            node_list.push_back(new TNode<T>(n[x].data());
    }

    TNodeList<T>& operator=(const TNodeList<T>& n) {
        if (n != this) {
            for(int x = 0; x < node_list.size(); x++)
                
            for(int x = 0; x < n.size(); x++)
                node_list.push_back(new TNode<T>(n[x].data());
        }
    }

    virtual ~TNodeList() {
        for(int x = 0; x < node_list.size(); x++)
            delete node_list[x];
    }

    TNode<T>* operator[](const int&n) {
        return nodes[n];
    }
    
    TNodeList& operator<<(const T& t) {
        node_list.push_back(&t);
        return *this;
    }

    void erase(const int& n) {
        node_list.erase(node_list.begin() + n);
    }

    void size() {
        return node_list.size();
    }
};



// TGraph description
template <class T>
class TGraph {
    TNodeList<T> node_list;

    TGraph(const TGraph<T>& g) { /*noncopyable*/ }
    TGraph<T>& operator=(const TGraph<T>& g) { /*noncopyable*/ }
public:
    TGraph() {
    }

    virtual ~TGraph() {
    }
    
    T& operator[](const int& n) {
        return nodes[n]->data();
    }

    int size() {
        return node_list.size();
    }

    void erase(const int& n) {
        node_list.erase(nodes.begin() + n);
    }

    TGraph& operator<< (const T& t) {
        node_list << t;
        return *this;
    }

    void add(const T& t) {
        node_list << t;
    }

    TNodeList<T>&  nodes() {
        return node_list;
    }

    std::string matrix() {
        std::string res = "";
        for(int x = 1; x < nodes.size(); x++) {
            for(int xx = 0; xx < node_list[x]->size()
        }
    }
};

#endif /* TGRAPH_H */
