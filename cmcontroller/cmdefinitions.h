#ifndef CMDEFINITIONS_H
#define CMDEFINITIONS_H
#include <string>
#include <util/socket/socket.h>

const int CM_DUMP_BUF_SIZE = 128;               // max request size
const float CMC_VERSION    = 0.61;               // cmc version
const std::string DD = ":";                     // host and target name separator

typedef TTCPSocket<1024> TCMSocket;

#endif 
