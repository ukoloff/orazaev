#include"cmc_server.h"
#include<util/logger/logger.h>

#include<iostream>
#include<iomanip>
#include<stdlib.h>
#include<getopt.h>

static float version = 0.35;

static std::string USAGE = "\
cmcontroll --- client-server application for cluster master tests.\n\
\n\
Usage:\n\
    cmcontroll --port NUM                                   : start cmcontroll server on port NUM.\n\
    cmcontroll --host NAME --port NUM [--request REQUEST]   : send REQUEST to server on host NAME and port NUM.\n\
\n\
Options:\n\
    -p --port     : set port.\n\
    -s --host     : set host.\n\
    -r --request  : set request to server.\n\
    -l --main-log : set main log. \n\
    -m --msg-log  : set message log. \n\
    -v --version  : print version.\n\
    -h --help     : print this message.";



void cmc_client_start(const std::string& host, const int& port, const std::string& request) {
    //connect to server
    //write request
    //get ans
}
    
void cmc_server_start(const int& port, const std::string& pidfilename, const std::string& mainLog, const std::string& msgLog) {
    int pid = fork();

    if (pid < 0) {
        std::cerr << "Error: can't fork server process." << std::endl;
        exit(2);
    }
    else if (pid == 0) {
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

        //while(true) {}
    }
}

void cmc_start(int argc, char** argv) {
    int opt;
    std::string host = "";
    int port = 0;
    std::string request = "";
    std::string pidfile = "";
    std::string mainLog = "";
    std::string msgLog  = "";
    
    while (1) {
        static struct option long_opts[] = {
            {"help",     no_argument,          0, 'h'},
            {"port",     required_argument,    0, 'p'},
            {"host",     required_argument,    0, 's'},
            {"request",  required_argument,    0, 'r'},
            {"version",  no_argument,          0, 'v'},
            {"pid",      required_argument,    0, 'P'},
            {"msg-log",  required_argument,    0, 'm'},
            {"main-log", required_argument,    0, 'l'},
            {0, 0, 0, 0}
        };

        int opt_index = 0;
    
        opt = getopt_long(argc, argv, "hvp:s:r:P:l:m:", long_opts, &opt_index);

        if (opt == -1)
            break;

        switch(opt) {
            case 'p': port = atoi(optarg); break;
            case 's': host    = std::string(optarg); break;
            case 'r': request = std::string(optarg); break;
            case 'P': pidfile = std::string(optarg); break;
            case 'm': msgLog  = std::string(optarg); break;
            case 'l': mainLog = std::string(optarg); break;
            case 'h': 
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
        std::cout << "Error: you must set port number to start server or host and port to start client" << std::endl;
        exit(1);
    }

    if (request != "" && host == "") {
        std::cout << "Error: server haven't request option" << std::endl;
    }

    if (host != "") {
        //client
        cmc_client_start(host, port, request);
    }
    else {
        //server
        cmc_server_start(port, pidfile, mainLog, msgLog);
    }
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
