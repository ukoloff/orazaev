#ifndef ASCIIART_H
#define ASCIIART_H

#include<stdio.h>
#include<iostream>
#include<cstring>
#include<cmath>

class TCharBrush {
public:
    TCharBrush();
    TCharBrush(const char c[5]);
    
    char& operator[](const unsigned& x) throw();
    unsigned size();
    
private:
    char z[5];
    static const unsigned sz = 5;
};

class TPlotWindow {
public:
    TPlotWindow(const unsigned& w, const unsigned& h, const unsigned& b_force = 0);
    TPlotWindow(const unsigned& w, const unsigned& h, const TCharBrush& cb, const unsigned& b_force = 0);
    virtual ~TPlotWindow();


    void setBrush(const TCharBrush& b);
    TCharBrush brush();

    void print();
    void dot(const int& x, const int& y, const unsigned& b_force = 4);
    void rectangle(const int& x, const int& y, const int& w, const int& h, const unsigned& b_force = 4);

private:
    unsigned H;
    unsigned W;
    char* matrix;
    TCharBrush char_brush;
    bool horizontal_X2_zoom;
};

#endif /* ASCIIART_H */
