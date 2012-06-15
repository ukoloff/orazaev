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
    TPlotWindow(const unsigned& w, const unsigned& h, const char& b = ' ');
    virtual ~TPlotWindow();

    void setHX2(const bool& b);

    void print();
    void dot(const int& x, const int& y, const char& b = '.');
    void rectangle(const int& x, const int& y, const int& w, const int& h, const char& b = '.');
    void line(int x0, int y0, int x1, int y1, const char& b = '.');

private:
    unsigned H;
    unsigned W;
    char* matrix;
    TCharBrush char_brush;
    bool horizontal_X2_zoom;
};

#endif /* ASCIIART_H */
