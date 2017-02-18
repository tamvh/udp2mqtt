#ifndef ZUDP_H
#define ZUDP_H
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>

class ZUdp
{
public:
    ZUdp();
    virtual ~ZUdp() {}
public:
    int initialize();
    void *callback(void *content);
private:
    int port = 8888;
};

#endif // ZUDP_H
