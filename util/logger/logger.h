#ifndef LOGGER_H
#define LOGGER_H

#include<fstream>
#include<iostream>
#include<stdlib.h>

class TLogger {
    std::string fname;
    bool echo;
    
    TLogger();

public:
    TLogger(std::string Fname, bool Echo = true)
        : fname(Fname)
        , echo(Echo) {
    }

    template<class T>
    TLogger& operator<<(const T& t) {
        if (!echo) return *this;

        std::ofstream logFile(fname.c_str(), std::ios_base::app);
        
        if (!logFile.is_open()) {
            std::cerr << "Error: Can't open file " << fname << " with append mode." << std::endl;
            exit(1);
        }

        logFile << t;
        
        logFile.close();
        return *this;
    }

    void off() { echo = 0; }
    void on()  { echo = 1; }
    
    void clear() {
        std::ofstream logFile(fname.c_str(), std::ios_base::out);

        if (!logFile.is_open()) {
            std::cerr << "Error: Can't open file " << fname << " with append mode." << std::endl;
            exit(1);
        }

        logFile.close();
    }
    
};

#endif
