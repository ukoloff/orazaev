#include "TSingleton.h"
#include "../tester/tester.h"

int main() {
    //TSingleton<int> test
    TSingleton<int> a(new int(23));
    TSingleton<int> b;

    //type singleton int
    typedef TSingleton<int> si;

    //type int pair
    typedef TPair<int, int> ip;

    ip t[8] = {
        ip(si(), 23),
        ip(++si(), 24),
        ip(si(new int(a - 24)), 0),
        ip(a, 0),
        ip(b, 0),
        ip((b += 5), 5),
        ip((a = b - 20), -15),
        ip(b, -15)
    };
    
    Tester::pntest(8, t);
}
