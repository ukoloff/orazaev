#include "compile_time_checker.h"


int main() {
    /// Wouldnt compile:
    // STATIC_ASSERT(2 + 2 == 5, INCORRECT_EXPRESSION);
    STATIC_ASSERT(2 + 3 == 5, CORRECT_EXPRESSION);

    return 0;
}
