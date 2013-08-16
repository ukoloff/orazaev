#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <map>
#include <memory>

class TRule;

typedef std::shared_ptr<TRule> (*TFun)();
typedef int TRuleId;


class TRule {
public:
    virtual void Do() = 0;

    template <typename T>
    static std::shared_ptr<TRule> CreateObject() {
        return std::shared_ptr<TRule>(new T);
    }

    virtual ~TRule() { }

protected:
    TRule() { }
};



class TRuleRegistry {
public:
    TRuleId Register(TFun pCreateFunction) {
        static TRuleId nextId = 0;
        registry[nextId++] = pCreateFunction;

        return nextId - 1;
    }

    std::shared_ptr<TRule> Create(TRuleId id) {
        std::map<TRuleId, TFun>::iterator iter = registry.find(id);
        if (iter == registry.end()) {
            throw std::logic_error("Unknown rule id!");
        }

        return iter->second();
    }

protected:
    std::map<TRuleId, TFun> registry;
};

TRuleRegistry& GetRegistery() {
    static TRuleRegistry registry;
    return registry;
}


class TFirstRule : public TRule {
public:
    virtual void Do() {
        printf("TFirstRule::Do()\n");
    }

protected:
    static TRuleId id;
};

class TSecondRule : public TRule {
public:
    virtual void Do() {
        printf("TSecondRule::Do()\n");
    }

protected:
    static TRuleId id;
};



TRuleId TFirstRule::id =
    GetRegistery().Register(TRule::CreateObject<TFirstRule>);

TRuleId TSecondRule::id =
    GetRegistery().Register(TRule::CreateObject<TSecondRule>);




int main() {
    GetRegistery().Create(1)->Do();
    GetRegistery().Create(0)->Do();

    return 0;
}
