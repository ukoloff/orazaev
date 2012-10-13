#include<cmcontroller/cmdefinitions.h>
#include<util/logger/logger.h>

#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<getopt.h>
#include<ctime>
#include<cstdio>

static float version = CMC_VERSION;

static std::string USAGE = "\
cmsender --- client application for cluster master tests.\n\
\n\
Usage:\n\
    cmsender -h HOST -p NUM [-l LOG] [-r REQ] [-t SEC]          : Send request REQ (default: read REQ from stdin) to HOST \n\
                                                                  on port NUM. Set connection timeout to SEC.\n\
    cmsender -v                                                 : get version of program. \n\
    cmsender -?                                                 : get help message. \n\
\n\
Options:\n\
    -p --port     : set port.\n\
    -h --host     : set host.\n\
    -l --log      : set main log. \n\
    -r --request  : set request(default request from stdin).\n\
    -t --timeout  : set timeout(default 15).\n\
    -v --version  : print version.\n\
    -? --help     : print this message.\n\
\n\
--------------------------------------------------------------------------------------------------------\n\
Requests syntax.(Warning: All requests without quetes!)\n\
    Addition: \n\
    '+ HOST: STARGET'                                           : +1 for non cluster target STARGET on host HOST.\n\
                                                                  It add in data next sentances:\n\
                                                                  *) 'HOST: STARGET'       --- all request.\n\
                                                                  *) 'STARGET'             --- only target name. \n\
    '+ HOST: TARGET.CLUSTER'                                    : +1 for target TARGET with cluster CLUSTER on host HOST.\n\
                                                                  It add in data next sentances:\n\
                                                                  *) 'HOST: TARGET.CLUSTER' --- all request.\n\
                                                                  *) 'TARGET.'              --- target name(with period).\n\
                                                                  *) 'TARGET.CLUSTER'       --- target name with cluster.\n\
                                                                  *) 'HOST: TARGET.'        --- host with target name.\n\
    \n\
    Request count of complete targets: \n\
    '? HOST: NAME'                                              : If non cluster target NAME is complete in host HOST,\n\
                                                                  that request return 1, becouse count of complete\n\
                                                                  targets by this request is 1.(If hard, see addition).\n\
    '? HOST: NAME.'                                             : Get count of complete cluster targets NAME on host HOST.'\n\
    '? HOST: NAME.CLUSTER'                                      : Is cluster CLUSTER of target NAME complete on host HOST?\n\
                                                                  If it so, request get 1.\n\
    '? NAME'                                                    : Get count of non cluster target NAME execution on \n\
                                                                  all hosts.\n\
    '? NAME.'                                                   : Get count of all clusters of target NAME execution on\n\
                                                                  all hosts.\n\
    '? NAME.CLUSTER'                                            : Get count of all executions of NAME.CLUSTER on\n\
                                                                  all hosts.\n\
    \n\
    Dump data:\n\
    'd FNAME'                                                   : Dump server information about executed targets\n\
                                                                  to file FILENAME.\n\
    \n\
    If it's hard to understand what it is --- just ask me. (orazaev@)";


void cmc_send(const std::string& host, const int& port, std::string& request, const std::string& lfname, const int& timeout) {
    if (request == "") 
        return;
    if (request.size() > CM_DUMP_BUF_SIZE)
        request.resize(CM_DUMP_BUF_SIZE);
    
    TCMSocket connection(host, port);

    time_t start_time = time(0);
    while (true) {
        try {
            connection.Connect();
        }
        catch (TCMSocket::ESocket) {
            if (!timeout)
                continue;

            time_t now = time(0);
            if ((now - start_time) < timeout)
                continue;
            else {
                std::cerr << "Error: exit by timeout" << std::endl;     
                exit(2);
            }
        }
        break;
    }


    connection.Write(request);

    std::cout << connection.Read();

    connection.Close();
}

void cmsender_start(int argc, char** argv) {
    int opt;
    int portno = 0;
    int timeout = 15;
    std::string host = "";
    std::string logFileName = "";
    std::string request = "";
    
    while (1) {
        static struct option long_opts[] = {
            {"help",    no_argument,          0, '?'},
            {"host",    required_argument,    0, 'h'},
            {"port",    required_argument,    0, 'p'},
            {"version", no_argument,          0, 'v'},
            {"log",     required_argument,    0, 'l'},
            {"timeout", required_argument,    0, 't'},
            {0, 0, 0, 0}
        };

        int opt_index = 0;
    
        opt = getopt_long(argc, argv, "?vp:l:r:h:t:", long_opts, &opt_index);

        if (opt == -1)
            break;

        switch(opt) {
            case 'p': portno = atoi(optarg); break;
            case 'l': logFileName = std::string(optarg); break;
            case 'h': host = std::string(optarg); break;
            case 'r': request = std::string(optarg); break;
            case 't': timeout = atoi(optarg); break;
            case '?': 
                std::cout << USAGE << std::endl; 
                exit(0); 
                break;
            case 'v': 
                std::cout.flags(std::ios::fixed);
                std::cout << "cmcontroll version is " << std::setprecision(1) << version << std::endl; 
                exit(0); 
                break;
            default : 
                std::cout << "Error: Try --help" << std::endl; 
                exit(1);
        }
    }

    if (portno == 0) {
        std::cerr << "Error: you must set port number to send request" << std::endl;
        std::cerr << "Error: try --help or -?" << std::endl;
        exit(1);
    }
    
    if (host.empty()) {
        std::cerr << "Error: you must set host to send request" << std::endl;
        std::cerr << "Error: try --help or -?" << std::endl;
        exit(1);
    }

    if (request.empty()) {
        char c = getchar();
        while (!feof(stdin) && c != '\n') {
            request += c;
            c = getchar();
            //char c = std::cin.get();
            //request += ((c == '' || c == '\n' || c == '\r') ? ' ' : c);
        }
    
        if (request[request.size() - 1] == '')
            request.resize(request.size() - 1);
    }

    cmc_send(host, portno, request, logFileName, timeout);
}

int main(int argc, char** argv) {
    try {
        cmsender_start(argc, argv);
    }
    catch (...) {
        std::cerr << "Something goes wrong" << std::endl;
        throw;
    }

}
