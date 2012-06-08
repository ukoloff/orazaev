#include<util/graph/tgraph.h>
#include<util/graph/tgraph.h>
#include<string>

std::string addSpaces(std::string s, int x = 50) {
    if (s.size() < x )
        s.resize(x, '.');
    return s;
}

std::string isTrue(bool expr) {
    if (expr)
        return std::string(" [ PASS ]\n");
    return std::string(" [ FAIL ]\n");
}

std::string isFalse(bool expr) {
    return isTrue(!expr);
}

void func(TNodeList<std::string> nls) {
    (nls[0])->data() = "a data";
}

void func(TPtr<TNode<std::string> > ns) {
    ns->data() = "b data";
}

int main() {
    typedef TNode<std::string> TNS;
    
    std::cout << addSpaces("construction");
    TNodeList<std::string> nlist;
    TPtr<TNS> a(new TNS("data in string node"));
    TPtr<TNS> b(new TNS());
    std::cout << isTrue(nlist.size() == 0 && a->edges().size() == 0 && a->data() == "data in string node" && b->data() == "" && b->edges().size() == 0);



    std::cout << addSpaces("operator<<() AND data()");
    nlist << a << b;
    nlist[1]->data() = "add b data";
    nlist << TPtr<TNS>(new TNS("without char"));
    std::cout << isTrue(nlist.size() == 3 && nlist[0]->data() == "data in string node" && nlist[1]->data() == "add b data" && b->data() == "add b data" && nlist[2]->data() == "without char");


    
    std::cout << addSpaces("copy & =");    
    func(nlist);
    func(b);
    nlist = nlist;
    TNodeList<std::string> *nlist2 = new TNodeList<std::string>();
    *nlist2 = nlist;
    std::cout << isTrue(nlist2->size() == nlist.size() && (*nlist2)[0].get() == nlist[0].get() && nlist[2]->data() == (*nlist2)[2]->data() && nlist[0]->data() == "a data" && nlist[1]->data() == "b data" && b->data() == "b data" && a->data() == "a data");



    std::cout << addSpaces("destructor");
    {
        TNodeList<int> ilist;
        TNode<int> ia;
        ilist << TPtr<TNode<int> >(new TNode<int>(3)) << TPtr<TNode<int> >(new TNode<int>(ia));
    }
    delete nlist2;
    std::cout << isTrue(nlist.size() == 3 && nlist[0]->data() == "a data" && nlist[2]->data() == "without char");
    


    std::cout << addSpaces("erase()");
    nlist.erase(0);
    std::cout << isTrue(nlist[0]->data() == "b data" && nlist[1]->data() == "without char" && nlist.size() == 2 && a->data() == "a data");



    std::cout << addSpaces("find()");
    int xx  = nlist.find(TPtr<TNS>(new TNS("")));
    int xxx = nlist.find(TNS(""));
    int xb  = nlist.find(TNS("b data"));
    int xxb = nlist.find(nlist[0]);
    int xw  = nlist.find(TNS("without char"));
    int xxw = nlist.find(nlist[1]);
    int rx  = nlist.rfind(TPtr<TNS>(new TNS("")));
    int rxx = nlist.rfind(TNS(""));
    int rb  = nlist.rfind(TNS("b data"));
    int rxb = nlist.rfind(nlist[0]);
    int rw  = nlist.find(TNS("without char"));
    int rxw = nlist.find(nlist[1]);
    std::cout << isTrue(xx == -1 && rx == -1 && xxx == -1 && rxx == -1 && xb == 0 && xxb == 0 && rb == 0 && rxb == 0 && xw == 1 && rw == 1 && xxw == 1 && rxw == 1);

    
    
    std::cout << addSpaces("edges() = ");
    a->edges() = nlist;
    std::cout << isTrue(a->edges().size() == nlist.size() && a->edges()[0]->data() == nlist[0]->data() && a->edges()[1]->data() == nlist[1]->data());

    return 0;
}






