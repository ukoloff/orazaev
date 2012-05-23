#ifndef LOGGER_H
#define LOGGER_H

#include<fstream>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<cstring>

namespace log {
    const std::string endl("\n\0x01");

    std::string NOW();
}

class TLogger {
    std::string fname;
    bool echo;
    bool timeEcho;
    
    TLogger();

public:
    TLogger(std::string Fname, bool Echo = true)
        : fname(Fname)
        , echo(Echo)
        , timeEcho(true) {
    }

    TLogger& operator<<(const std::string& s) {
        if (!echo) return *this;

        std::ofstream logFile(fname.c_str(), std::ios_base::app);
        
        if (!logFile.is_open()) {
            std::cerr << "Error: Can't open file " << fname << " with append mode." << std::endl;
            exit(1);
        }

        if (timeEcho) { 
            logFile << log::NOW();
            timeEcho = false;
        }

        if (s == log::endl) {
            logFile << std::endl;
            timeEcho = true;
        }
        else
            logFile << s;
        
        logFile.close();
        return *this;
    }

    template<class T>
    TLogger& operator<<(const T& t) {
        if (!echo) return *this;

        std::ofstream logFile(fname.c_str(), std::ios_base::app);
        
        if (!logFile.is_open()) {
            std::cerr << "Error: Can't open file " << fname << " with append mode." << std::endl;
            exit(1);
        }

        if (timeEcho) { 
            logFile << log::NOW();
            timeEcho = false;
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
