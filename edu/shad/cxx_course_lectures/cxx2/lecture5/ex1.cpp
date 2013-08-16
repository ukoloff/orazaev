#include <iostream>
#include <stdlib.h>

class TShape {
public:
    virtual void Draw() { }
    void RttiDraw();

    virtual ~TShape() { }

protected:
    TShape() { }
};

class TSquare : public TShape {
public:
    TSquare() { }

    virtual void Draw() { /*std::cout << "[ ]\n";*/ }
};

class TTriangle : public TShape {
public:
    TTriangle() { }

    virtual void Draw() { /*std::cout << "/\\\n";*/ }
};

class TCircle : public TShape {
public:
    TCircle() { }

    virtual void Draw() { /*std::cout << "o\n";*/ }
};

void TShape::RttiDraw() {
    if (dynamic_cast<TSquare*>(this)) {
        //std::cout << "RTTI: [ ]\n";
        return;
    }

    if (dynamic_cast<TTriangle*>(this)) {
        //std::cout << "RTTI: /\\\n";
        return;
    }

    if (dynamic_cast<TCircle*>(this)) {
        //std::cout << "RTTI: o\n";
        return;
    }
}



class TTimer {
    clock_t startClocks;
    clock_t endClocks;

    public:
    TTimer()
        : startClocks(0)
        , endClocks(0) {
    }

    void Start() {
        startClocks = clock();
    }

    void Stop() {
        endClocks = clock();
    }

    double GetSeconds() const {
        return static_cast<double>(
            endClocks - startClocks) / CLOCKS_PER_SEC;
    }

    clock_t GetClocks() const {
        return endClocks - startClocks;
    }
};



int main() {
    TShape* shape[3];

    shape[0] = new TSquare();
    shape[1] = new TTriangle();
    shape[2] = new TCircle();

    TTimer theTimer;

    theTimer.Start();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 1000000; ++j){
            shape[i]->Draw();
        }
    }
    theTimer.Stop();
    std::cout << "VIRTUAL TIME: " << theTimer.GetSeconds() << std::endl;

    theTimer.Start();
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 1000000; ++j){
            shape[i]->RttiDraw();
        }
    }
    theTimer.Stop();
    std::cout << "RTTI TIME: " << theTimer.GetSeconds() << std::endl;

    delete shape[0];
    delete shape[1];
    delete shape[2];

    return 0;
}
