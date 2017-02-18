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
#include <zudp.h>
#include <zmqtt.h>
using namespace std;
static int connFd;
std::string mqtt_topic = "esp32";
ZUdp *udp;
ZMqtt *mqtt;
void post_to_mqtt(const std::string& msg) {
    mqtt->publish(mqtt_topic, msg);
}
void init_mqtt(){
    std::string mqtt_clientId = "";
    std::string mqtt_host = "localhost";
    int mqtt_port = 1883;

    mqtt = new ZMqtt(mqtt_clientId, mqtt_host, mqtt_port);
    mqtt->preSubscribe(mqtt_topic, 0);
    mqtt->autoReconnect(true);
    mqtt->beginConnect();
    mqtt->connect();
}
void *callback (void *content)
{
    cout << "Thread No: " << pthread_self() << endl;
    char buffer[1024];
    while(true)
    {
        bzero(buffer, sizeof(buffer) + 1);
        read(connFd, buffer, 300);
        std::string msg (buffer);
        cout << "Message: " << msg << endl;
        //push to mqtt server
        if(!msg.empty())
            post_to_mqtt(msg);
    }
    cout << "\nClosing thread and conn" << endl;
    close(connFd);
}

int main(int argc, char* argv[])
{
    init_mqtt();
    int listenFd;
    socklen_t len; //store size of the address
    struct sockaddr_in clntAdd;
    pthread_t threadA;
    udp = new ZUdp();
    listenFd = udp->initialize();
    listen(listenFd, 5);
    len = sizeof(clntAdd);
    while (1)
    {
        cout << "UDP Listening" << endl;
        connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);

        if (connFd < 0)
        {
            cerr << "Cannot accept connection" << endl;
            return 0;
        }
        else
        {
            cout << "Connection successful" << endl;
        }
        pthread_create(&threadA, NULL, callback, NULL);
    }
    pthread_join(threadA, NULL);
}


