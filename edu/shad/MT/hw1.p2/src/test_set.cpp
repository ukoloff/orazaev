#include <set.h>
#include <cassert>

int main() {
    TSynchronizedSet<int> set;

    assert(set.Insert(0));
    assert(!set.Insert(0));
    assert(set.Insert(1));
    assert(!set.Insert(0));
    assert(set.Insert(2));
    assert(!set.Insert(1));
    assert(!set.Insert(0));

    return 0;
}
