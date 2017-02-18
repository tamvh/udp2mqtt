#include "zudp.h"
using namespace std;
ZUdp::ZUdp()
{
}

int ZUdp::initialize() {
    int listenFd;
    struct sockaddr_in svrAdd;

    //create socket
    listenFd = socket(AF_INET, SOCK_STREAM, 0);

    if(listenFd < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }

    bzero((char*) &svrAdd, sizeof(svrAdd));

    svrAdd.sin_family = AF_INET;
    svrAdd.sin_addr.s_addr = INADDR_ANY;
    svrAdd.sin_port = htons(port);

    //bind socket
    if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0)
    {
        cerr << "Cannot bind" << endl;
        return 0;
    }
    return listenFd;
}

