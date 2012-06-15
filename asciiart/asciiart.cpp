#include"asciiart.h"

/* TCharBrush description */
TCharBrush::TCharBrush() {
    z[0] = ' ';
    z[1] = '.';
    z[2] = '-';
    z[3] = '+';
    z[4] = '@';
}

TCharBrush::TCharBrush(const char c[5]) {
    for(int x = 0; x < 5; x++)
        z[x] = c[x];
}

char& TCharBrush::operator[](const unsigned& x) throw() {
    if (x < 5)
        return z[x];
    return z[0];
}

unsigned TCharBrush::size() {
    return this->sz;
}
 
/* TPlotWindow description */
TPlotWindow::TPlotWindow(const unsigned& w, const unsigned& h, const char& b)
    : H(h)
    , W(w)
    , matrix(0)
    , char_brush()
    , horizontal_X2_zoom(false) {
    matrix = new char[W * H];
    memset(matrix, b, W * H);
}

TPlotWindow::~TPlotWindow() {
    delete[] matrix;
}

void TPlotWindow::setHX2(const bool& b) {
    horizontal_X2_zoom = b;
}

void TPlotWindow::print() {
    for(unsigned y = 0; y < H; y++) {
        for(unsigned x = 0; x < W; x++) {
            std::cout << matrix[x * H + y];
            if (horizontal_X2_zoom)
                std::cout << ' ';
        }
    std::cout << std::endl;
    }
}

void TPlotWindow::dot(const int& x, const int& y, const char& b) {
    if (x >= 0 && y >= 0 && x * H + y < W * H)
        matrix[x * H + y] = b;
}

void TPlotWindow::rectangle(const int& x, const int& y, const int& w, const int& h, const char& b) {
    if (w < 0 || h < 0) {
        int new_x = w < 0 ? x + w + 1 : x;
        int new_y = h < 0 ? y + h + 1 : y;
        
        this->rectangle(new_x, new_y, std::abs(w), std::abs(h));
        return;
    }

    for(int p = x; p < x + w ; p++) {
        if (p * H + y < W * H && p >= 0 && y >= 0)
            matrix[p * H + y] = b;
        if (p * H + y + h - 1 < W * H && p >= 0 && y + h - 1 >= 0)
            matrix[p * H + y + h - 1] = b;
    }

    for(int p = y; p < y + h ; p++) {
        if (x * H + p < W * H && p >= 0 && x >= 0)
            matrix[x * H + p] = b;
        if ((x + w - 1) * H + p < W * H && p >= 0 && x + w - 1 >= 0)
            matrix[(x + w - 1) * H + p] = b;
    }
}

void TPlotWindow::line(int x0, int y0, int x1, int y1, const char& b) {
/**
TPlotWindow::line(x0, y0, x1, y1) --- draw line from (x0, y0) to (x1, y1)

Use Bresenham algorithm: 
http://en.wikipedia.org/wiki/Bresenham_algorithm
*/
    const int dx    = std::abs(x1 - x0);
    const int dy    = std::abs(y1 - y0);
    const int signx = x0 < x1 ? 1 : -1;
    const int signy = y0 < y1 ? 1 : -1;
    
    int error = dx - dy;

    this->dot(x1, y1, b);
    
    while(x0 != x1 || y0 != y1) {
        this->dot(x0, y0, b);
        const int error2 = error * 2;
        if (error2 > -dy) {
            error -= dy;
            x0 += signx;
        }

        if (error2 < dx) {
            error += dx;
            y0 += signy;
        }
    }
}


