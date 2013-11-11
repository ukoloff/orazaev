/**
 * Monti-hall paradox trivial simulation.
 *
 * http://en.wikipedia.org/wiki/Monty_Hall_problem
 */
#include <iostream>
#include <cstdlib>

class Doors {
public:
    Doors()
        : ans_(std::rand() % 3)
    { }

    /**
     * simplification after code refactoring :)
     */
    bool simulate() {
        size_t choose = std::rand() % 3;
        return choose != ans_;
    }

private:
    size_t ans_;
};

int main() {
    std::srand(42);

    size_t times = 10000;
    size_t win = 0;
    for (size_t i = 0; i < times; ++i) {
        win += Doors().simulate();
    }

    std::cout << double(win) / times << std::endl;

    return 0;
}
