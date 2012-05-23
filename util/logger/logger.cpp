#include "logger.h"

std::string log::NOW() {
    time_t x;
    time(&x);
    
    std::string res = "[ " + std::string(ctime(&x));
    res.resize(res.size() - 1);
    res += " ] ";
    
    return res;
}
