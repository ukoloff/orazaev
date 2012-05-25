#include"cmc_server.h"
#include<util/logger/logger.h>

#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<getopt.h>

static float version = CMC_VERSION;

static std::string USAGE = "\
cmcontroll --- server application for cluster master tests.\n\
\n\
Usage:\n\
    cmcontroll --port NUM [-l LOG] [-m MSG] [-P PID]            : start cmcontroll server on port NUM.\n\
    cmcontroll -v                                               : get version of program. \n\
    cmcontroll -?                                               : get help message. \n\
\n\
Options:\n\
    -p --port     : set port.\n\
    -l --main-log : set main log. \n\
    -m --msg-log  : set message log. \n\
    -P --pid-file : set pid file. \n\
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



void cmc_server_start(const int& port, const std::string& pidfilename, const std::string& mainLog, const std::string& msgLog) {
    int pid = fork();

    if (pid < 0) {
        std::cerr << "Error: can't fork server process." << std::endl;
        exit(2);
    }
    else if (pid != 0) {
        // main process save pid
        if (pidfilename == "") return;

        std::ofstream pfile(pidfilename.c_str());
   
        if (!pfile.is_open()) {
            std::cerr << "Error: can't open file '" << pidfilename << "' to save pid." << std::endl;
            return;
        }

        pfile << pid;
        pfile.close();
    }
    else {
        // start server in child process
        TCMServer serv(port);

        if (!mainLog.empty()) serv.setMainLog(mainLog);
        if (!msgLog.empty())  serv.setMsgLog(msgLog);
        
        serv.startListen();
    }
}

void cmc_start(int argc, char** argv) {
    int opt;
    int port = 0;
    std::string pidfile = "";
    std::string mainLog = "";
    std::string msgLog  = "";
    
    while (1) {
        static struct option long_opts[] = {
            {"help",     no_argument,          0, '?'},
            {"port",     required_argument,    0, 'p'},
            {"version",  no_argument,          0, 'v'},
            {"pid",      required_argument,    0, 'P'},
            {"msg-log",  required_argument,    0, 'm'},
            {"main-log", required_argument,    0, 'l'},
            {0, 0, 0, 0}
        };

        int opt_index = 0;
    
        opt = getopt_long(argc, argv, "?vp:P:l:m:", long_opts, &opt_index);

        if (opt == -1)
            break;

        switch(opt) {
            case 'p': port = atoi(optarg); break;
            case 'P': pidfile = std::string(optarg); break;
            case 'm': msgLog  = std::string(optarg); break;
            case 'l': mainLog = std::string(optarg); break;
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

    if (port == 0) {
        std::cout << "Error: you must set port number to start server" << std::endl;
        exit(1);
    }

    cmc_server_start(port, pidfile, mainLog, msgLog);
}

int main(int argc, char** argv) {
    try {
        cmc_start(argc, argv);
    }
    catch (...) {
        std::cerr << "Something goes wrong" << std::endl;
        throw;
    }

}
