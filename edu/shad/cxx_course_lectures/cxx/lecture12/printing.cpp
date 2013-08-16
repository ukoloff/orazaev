#include <iostream>

class Formatter {
    public:
    virtual void printHeader() const = 0;
    virtual void printTitle() const = 0;
    virtual void printParagraph() = 0;
    virtual bool eof() const = 0;
    virtual void printFooter() const = 0;

    virtual ~Formatter() {
    }
};



class PlainTextFormatter : public Formatter {
    bool isEof;
    public:
    PlainTextFormatter()
        : isEof(false) {
    }

    void printHeader() const {
        std::cout << "PlainTextFormatter::printHeader()" << std::endl;
    }

    void printTitle() const {
        std::cout << "PlainTextFormatter::printTitle()" << std::endl;
    }

    void printParagraph() {
        std::cout << "PlainTextFormatter::printParagraph()" << std::endl;
        isEof = true;
    }

    bool eof() const {
        return isEof;
    }

    void printFooter() const {
        std::cout << "PlainTextFormatter::printFooter()" << std::endl;
    }
};





class HtmlFormatter : public Formatter {
    bool isEof;
    public:
    HtmlFormatter()
        : isEof(false) {
    }

    void printHeader() const {
        std::cout << "HtmlFormatter::printHeader()" << std::endl;
    }

    void printTitle() const {
        std::cout << "HtmlFormatter::printTitle()" << std::endl;
    }

    void printParagraph() {
        std::cout << "HtmlFormatter::printParagraph()" << std::endl;
        isEof = true;
    }

    bool eof() const {
        return isEof;
    }

    void printFooter() const {
        std::cout << "HtmlFormatter::printFooter()" << std::endl;
    }
};





void printDocument(Formatter& f) {
    f.printHeader();
    f.printTitle();
    while (!f.eof())
        f.printParagraph();
    f.printFooter();
};


int main() {
    PlainTextFormatter ptf;
    HtmlFormatter hf;

    printDocument(ptf);
    std::cout << "------------------------------------------" << std::endl;
    printDocument(hf);
    return 0;
}
