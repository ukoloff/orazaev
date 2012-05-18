#include"logger.h"
#include<string>

class A {
public:
    A() {};
};

std::ostream& operator<<(std::ostream& os, const A& a) {
    os << "type a";
    return os;
}

int main() {
    TLogger l("logfile.log");

    l.clear();

    l << 0 << "\n";
    l << 0 << " " << std::string("one") << ' ' << "two" << std::string(" ") << "\n";
    
    A a;
//    l << a << std::endl;

    l << "test";

    l << " end";

    return 0;
}
