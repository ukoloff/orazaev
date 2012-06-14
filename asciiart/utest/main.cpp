#include"asciiart/asciiart.h"

int main() {
    TPlotWindow tp(25, 6, 1);
    tp.rectangle(0, 0, 25, 6, 3);
    tp.dot(1, 1);
    tp.print();
    tp.dot(1, 2);
    tp.print();
    tp.dot(1, 3);
    tp.print();
    tp.dot(1, 4);
    tp.print();
    tp.dot(1, 5);
    tp.print();
    
    return 0;
}
