#include<iostream>
#include<vector>

class TObject {
    public:
    TObject() {
        std::cout << "TObject::Default constructor for id = "
            << (int) this << std::endl;
    }

    TObject(const TObject & other) {
        std::cout << "TObject::Copy constructor for id = "
            << (int) this << std::endl;
    }

    ~TObject() {
        std::cout << "TObject::Destructor for "
            << (int) this << std::endl;
    }

    TObject & operator == (const TObject & rhs) {
        std::cout << "TObject::Operator == for id = "
            << (int) this << std::endl;
    }
};

int main() {
    {
    std::vector<TObject> objVector(10, TObject());
    }

    TObject * x = new TObject[10];
    delete [] x;
    return 0;
}
