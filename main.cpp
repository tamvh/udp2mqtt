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
#include <zmqtt.h>
#define BUFLEN 512
#define PORT 8888
std::string mqtt_topic = "esp32";
ZMqtt *mqtt;
using namespace std;
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

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    init_mqtt();
    struct sockaddr_in si_me, si_other;
    int s, recv_len;
    char buf[BUFLEN];
    socklen_t slen = sizeof(si_other);
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die((char*)"socket");
    }

    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die((char*)"bind");
    }

    //keep listening for data
    while(1)
    {
        printf("Waiting for data...");
        fflush(stdout);
        memset((char *) &buf, 0, sizeof(buf));
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die((char*)"recvfrom()");
        }
        printf("Data: %s\n" , buf);
        std::string msg(buf, sizeof(buf));
        post_to_mqtt(msg);
    }
    close(s);
    return 0;
}


