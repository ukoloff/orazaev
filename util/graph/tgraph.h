#ifndef TGRAPH_H
#define TGRAPH_H
#include<vector>
#include<map>
#include<list>
#include<memory>
#include<iostream>
#include<util/memory/tptr.h>

template<class T>
class TNodeList;


// TNode description
template<class T>
class TNode {
    T node_data;
    std::vector<TNode<T>*> node_edges;

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

    std::vector<TNode<T>*>& edges() { 
        return node_edges; 
    }

    TNode<T>* edge(const unsigned& n) {
        return node_edges[n];
    }

    TNode<T>* operator[](const unsigned& n) {
        return node_edges[n];
    }

    unsigned size() const {
        return node_edges.size();
    }
};

template<class T>
std::vector<TNode<T>*>& operator<<(std::vector<TNode<T>*>& nvector, TPtr<TNode<T> > pn) {
    nvector.push_back(pn.get());
    return nvector;
}


// TNodeList description
template<class T>
class TNodeList {
    std::vector<TPtr<TNode<T> > > node_list;
    
public:
    TNodeList() {
    }

    virtual ~TNodeList() {
    }

    TPtr<TNode<T> >& operator[](const int&n) throw() {
        return node_list[n];
    }
    
    virtual TNodeList& operator<<(const TPtr<TNode<T> >& pn) throw() {
        node_list.push_back(pn);
        return *this;
    }

    virtual void erase(const int& n) throw() {
        node_list.erase(node_list.begin() + n);
    }

    unsigned size() const throw() {
        return node_list.size();
    }

    int find(TPtr<TNode<T> > pn, int start_pos = 0) throw() {
        for(int x = start_pos; x < node_list.size(); x++)
            if (pn->data() == node_list[x]->data())
                return x;
        return -1;
    }

    int find(TNode<T> n, int start_pos = 0) throw() {
        for(int x = start_pos; x < node_list.size(); x++)
            if (n.data() == node_list[x]->data())
                return x;
        return -1;
    }

    int rfind(TPtr<TNode<T> > pn) throw() {
        for(int x = node_list.size() - 1; x >= 0; x--)
            if (pn->data() == node_list[x]->data())
                return x;
        return -1;
    }
    
    int rfind(TNode<T> n) throw() {
        for(int x = node_list.size() - 1; x >= 0; x--)
            if (n.data() == node_list[x]->data())
                return x;
        return -1;
    }
};


// TGraph description
template<class T>
class TGraph {
    TNodeList<T> node_list;
    
public:
    TGraph() 
    : node_list() {
    }

    virtual ~TGraph() {
    }
    
    TPtr<TNode<T> >& operator[](const int& n) {
        return node_list[n];
    }

    int size() {
        return node_list.size();
    }

    void erase(const int& n) {
        node_list.erase(node_list.begin() + n);
    }

    TGraph& operator<<(const T& t) {
        node_list << TPtr<TNode<T> >(new TNode<T>(t));
        return *this;
    }
    
    TGraph& operator<<(const TNode<T>& n) {
        node_list << TPtr<TNode<T> >(new TNode<T>(n));
        return *this;
    }

    void add(const T& t) {
        node_list << TPtr<TNode<T> >(new TNode<T>(t));
    }

    void add(const TNode<T>& n) {
        node_list << TPtr<TNode<T> >(new TNode<T>(n));
    }

    TNodeList<T>&  nodes() {
        return node_list;
    }
};


namespace NGraph {
    template<class T>
    void printGraphMatrix(TGraph<T> g) {
    }
    
    template<class T>
    void printGraph(TGraph<T>& g, std::ostream& os = std::cout) {
        for(int x = 0; x < g.size(); x++){
            os << x << " : ";
            for(int xx = 0; xx < g[x]->size(); xx++) {
                int pos = g.nodes().find(g[x]->edge(xx));
                if (pos != -1)
                    os << "(" << pos << ", " << g[pos]->data() << ") ";
            }
            os << std::endl;
        }
    }
}

#endif /* TGRAPH_H */
