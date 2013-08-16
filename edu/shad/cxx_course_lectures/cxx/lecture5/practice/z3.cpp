#include<iostream>
#include<vector>
#include<stack>
#include<cstdlib>

size_t getOperationNo(const std::string & s) {
    if (s == "+")
        return 1;
    else if (s == "-")
        return 2;
    else if (s == "*")
        return 3;
    else if (s == "/")
        return 4;

    return 0;
}

int doOperation(size_t operationNo, int a, int b) {
    switch(operationNo) {
        case 1: return b + a;
        case 2: return b - a;
        case 3: return b * a;
        case 4: return b / a;
    }
    return 0;
}

int getResult(std::stack<std::string> * s, size_t opNo) {
    std::string sarg1 = s->top(); s->pop();
    int arg1 = 0;
    if (getOperationNo(sarg1)) {
        arg1 = getResult(s, getOperationNo(sarg1));
    } else {
        arg1 = atoi(sarg1.c_str());
    }

    std::string sarg2 = s->top(); s->pop();
    int arg2 = 0;
    if (getOperationNo(sarg2)) {
        arg2 = getResult(s, getOperationNo(sarg2));
    } else {
        arg2 = atoi(sarg2.c_str());
    }

    return doOperation(opNo, arg1, arg2);
}

int calc(const std::vector<std::string> & postfix) {
    std::stack<std::string> s;
    int ans;

    for(std::vector<std::string>::const_iterator it = postfix.begin();
        it != postfix.end(); ++it) {
        s.push(*it);
    }


    if (!s.empty()) {
        std::string op = s.top(); s.pop();
        if (getOperationNo(op))
            return getResult(&s, getOperationNo(op));
        else 
            return atoi(op.c_str());
    }

    return 0;
}

int main() {
    std::vector<std::string> v;
    
    while(std::cin.good()) {
        std::string x;
        std::cin >> x;
        
        v.push_back(x);
    }

    std::cout << "\nans: " << calc(v) << std::endl;
    return 0;
}
