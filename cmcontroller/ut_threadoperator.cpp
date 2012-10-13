#include<iostream>
#include"cmc_server.h"
#include"../util/tester/tester.h"

class UThreadOperator : private TCMServer::TThreadOperator {
    inline std::string wits(int x) {
        std::string res;
        std::cout << "int(" << x << ") = ";
        res = its(x);
        std::cout << "'" << res << "'" << std::endl;

        return res;
    }

    inline std::string wgetRequest() {
        std::cout << "GetRequest() = ";

        return res;
    }

    inline std::string wgetRequestHost(std::string request) {
        std::string res;
        std::cout << "getRequestHost('" << request << "') = ";
        res = getRequest(request);
        std::cout << "'" << res << "'" << std::endl;

        return res;
    }

    inline std::string wgetRequestTargetName(std::string request) {
        std::string res;
        std::cout << "getRequestTargetName('" << request << "') = ";
        res = getRequestTargetName(request);
        std::cout << "'" << res << "'" << std::endl;

        return res;
    }

    inline std::string wgetRequestCluster(std::string request) {
        std::string res;
        std::cout << "getRequestCluster('" << request << "') = ";
        res = getRequestCluster(request);
        std::cout << "'" << res << "'" << std::endl;
        
        return res;
    }


public:
    UThreadOperator(int portno, std::map<std::string, int>* ppd) : TTHreadOperator(TCMSocket(portno), 0) {
        pdata = new std::map<std::string, int>();
    }

    virtual ~UThreadOperator() {
        delete pdata;
    }

    bool test_getRequestHost() {
        typedef TPair<std::string, std::string> ptt;
        ptt k[10] = {
            ptt(wgetRequestHost(""), ""),
            ptt(wgetRequestHost("twalrus: target_name"), "target_name"),
            ptt(wgetRequestHost(": xxx__xxx"), "xxx__xxx"),
            ptt(wgetRequestHost("twalrus2: yyy"), "yyy"),
            ptt(wgetRequestHost("dech000.yandex.ru: yyy.0"), "yyy."),
            ptt(wgetRequestHost("sub.sub.sub.ru: name.xxxx"), "name."),
            ptt(wgetRequestHost("lenta.ru: fuck_human_brain.complete"), "fuck_human_brain."),
            ptt(wgetRequestHost("president.ru: "), ""),
            ptt(wgetRequestHost("duck.de: ."), "."),
            ptt(wgetRequestHost("yandex.ru : ok."), "ok."),
        };

        Tester::pntest(10, k);
    
        return ntest(10, k);
    }

    bool test_getRequestTargetName() {

    }

    bool test_getRequestCluster() {

    }
    
};


int main() {
    
    UThreadOperator x;
    x.test_getRequestHost();
    
    return 0;
}
