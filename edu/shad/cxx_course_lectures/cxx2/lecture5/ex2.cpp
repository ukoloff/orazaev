#include <iostream>
#include <string>
#include <string.h>

class IPerson {
public:
    virtual ~IPerson() { }
    virtual std::string Name() const = 0;
    virtual std::string BirthDate() const = 0;

};

class TStudentInfo {
public:
    TStudentInfo(
        const std::string& name,
        const std::string& bdate)
        : _name(name)
        , _bdate(bdate)
    { }

    const char * GetName() const {
        result = ValueOpenDelimeter();
        result += _name;
        result += ValueCloseDelimeter();

        return result.c_str();
    }

    const char * GetDate() const {
        result = "";
        std::string date = _bdate;

        unsigned pos;
        for (pos = date.find("/");
             pos != std::string::npos;
             pos = date.find("/"))
        {
            result += date.substr(0, pos) + DateSeparator();
            date = date.substr(pos + 1);
        }
        result += date.substr(0, pos);

        return result.c_str();
    }

public:
    virtual const char * ValueOpenDelimeter() const { return "[<"; }
    virtual const char * ValueCloseDelimeter() const { return ">]"; }
    virtual const char * DateSeparator() const { return "$$"; }

protected:
    const std::string _name;
    const std::string _bdate;

private:
    mutable std::string result;
};


class TStudent : public IPerson, public TStudentInfo {
public:
    TStudent(
        const std::string& name,
        const std::string& bdate)
        : TStudentInfo(name, bdate)
    { }

    virtual std::string Name() const {
        std::string result = GetName();
        result = result.substr(strlen(ValueOpenDelimeter()));
        result = result.substr(0, result.size() - strlen(ValueCloseDelimeter()));

        return result;
    }

    virtual std::string BirthDate() const {
        std::string date = GetDate();
        std::string result;

        unsigned pos;
        for (pos = date.find(DateSeparator());
             pos != std::string::npos;
             pos = date.find(DateSeparator()))
        {
            result += date.substr(0, pos) + "/";
            date = date.substr(pos + strlen(DateSeparator()));
        }
        result += date.substr(0, pos);

        return result;
    }
};


class TStudentComposition : public IPerson {
public:
    TStudentComposition(
        const std::string& name,
        const std::string& bdate)
        : _si(name, bdate)
    { }

    virtual std::string Name() const {
        std::string result = _si.GetName();
        result = result.substr(strlen(_si.ValueOpenDelimeter()));
        result = result.substr(0, result.size() - strlen(_si.ValueCloseDelimeter()));

        return result;
    }

    virtual std::string BirthDate() const {
        std::string date = _si.GetDate();
        std::string result;

        unsigned pos;
        for (pos = date.find(_si.DateSeparator());
             pos != std::string::npos;
             pos = date.find(_si.DateSeparator()))
        {
            result += date.substr(0, pos) + "/";
            date = date.substr(pos + strlen(_si.DateSeparator()));
        }
        result += date.substr(0, pos);

        return result;
    }

private:
    TStudentInfo _si;
};

int main() {
     TStudent sasha("Sasha", "02/06/1987");
     TStudentComposition sasha2("Sasha", "02/06/1987");
    TStudentInfo sashaInfo("Sasha", "02/06/1987");

    std::cout << sashaInfo.GetName() << std::endl;
    std::cout << sashaInfo.GetDate() << std::endl;

     std::cout << sasha.Name() << std::endl;
     std::cout << sasha.BirthDate() << std::endl;

     std::cout << sasha2.Name() << std::endl;
     std::cout << sasha2.BirthDate() << std::endl;
}
