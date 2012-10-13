#ifndef TERROR_H
#define TERROR_H
#include<string>
#include<exception>

class TError : public std::exception {
    std::string msg;
public:
    TError() throw()
        : msg("TError: Something goes wrong!")
    { }

    virtual ~TError() throw()
    { }

    TError(const std::string & s) throw()
        : msg(s)
    { }

    virtual const char * what() const throw() {
        return msg.c_str();
    }
};

#endif
