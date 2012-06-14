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
TPlotWindow::TPlotWindow(const unsigned& w, const unsigned& h, const unsigned& b_force)
    : H(h)
    , W(w)
    , matrix(0)
    , char_brush()
    , horizontal_X2_zoom(true) {
    matrix = new char[W * H];
    if (b_force > char_brush.size())
        memset(matrix, char_brush[0], W * H);
    else
        memset(matrix, char_brush[b_force], W * H);
}

TPlotWindow::TPlotWindow(const unsigned& w, const unsigned& h, const TCharBrush& cb, const unsigned& b_force)
    : H(h)
    , W(w)
    , matrix(0)
    , char_brush(cb)
    , horizontal_X2_zoom(true) {
    matrix = new char[W * H];
    if (b_force > char_brush.size())
        memset(matrix, char_brush[0], W * H);
    else
        memset(matrix, char_brush[b_force], W * H);
}

TPlotWindow::~TPlotWindow() {
    delete[] matrix;
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

void TPlotWindow::setBrush(const TCharBrush& b) {
    char_brush = b;
}

TCharBrush TPlotWindow::brush() {
    return char_brush;
}

void TPlotWindow::dot(const int& x, const int& y, const unsigned& b_force) {
    unsigned bb = b_force;
    if (bb > char_brush.size())
        bb = char_brush.size() - 1;

    if (x >= 0 && y >= 0 && x * H + y < W * H)
        matrix[x * H + y] = char_brush[bb];
}

void TPlotWindow::rectangle(const int& x, const int& y, const int& w, const int& h, const unsigned& b_force) {
    unsigned bb = b_force;
    if (bb > char_brush.size())
        bb = char_brush.size() - 1;

    if (w < 0 || h < 0) {
        int new_x = w < 0 ? x + w + 1 : x;
        int new_y = h < 0 ? y + h + 1 : y;
        
        this->rectangle(new_x, new_y, std::abs(w), std::abs(h));
        return;
    }

    for(int p = x; p < x + w ; p++) {
        if (p * H + y < W * H && p >= 0 && y >= 0)
            matrix[p * H + y] = char_brush[bb];
        if (p * H + y + h - 1 < W * H && p >= 0 && y + h - 1 >= 0)
            matrix[p * H + y + h - 1] = char_brush[bb];
    }

    for(int p = y; p < y + h ; p++) {
        if (x * H + p < W * H && p >= 0 && x >= 0)
            matrix[x * H + p] = char_brush[bb];
        if ((x + w - 1) * H + p < W * H && p >= 0 && x + w - 1 >= 0)
            matrix[(x + w - 1) * H + p] = char_brush[bb];
    }
}
