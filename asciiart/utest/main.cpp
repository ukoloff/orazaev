#include"asciiart/asciiart.h"

int main() {
    TCharMap tp(50, 50);
    tp.rectangle(11, 25, 27, 25, '.');
    tp.line(11, 25, 24, 12);
    tp.line(24, 12, 37, 25);
    tp.print();
    tp.setHX2(true);
    tp.print();
    
    return 0;
}
