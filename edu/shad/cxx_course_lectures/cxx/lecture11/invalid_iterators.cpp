#include <iostream>
#include <vector>
#include <list>
#include <map>

int main() {
    std::vector<int> v(10, 1);
    std::vector<int>::iterator vi = v.begin();

    std::cout << *vi << std::endl;
    v.push_back(2);
    std::cout << *vi << std::endl;

    std::list<int> l;
    l.push_back(1);
    l.push_back(2);
    std::list<int>::iterator li = l.begin();
    ++li;

    std::cout << *li << std::endl;
    l.clear();
    l.push_back(3);
    l.push_back(4);
    std::cout << *li << std::endl;


    std::map<int, int> m;
    m[3] = 9;
    m[4] = 16;

    std::map<int, int>::iterator mi = ++m.begin();
    std::cout << mi->first << " " << mi->second << std::endl;

    m.clear();

    std::cout << mi->first << " " << mi->second << std::endl;
    return 0;
}
