#ifndef _MQTTi_H
#define _MQTTi_H

#include "MQTTpacket.h"
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>

/*  Interface to MQTTpackets and internet sockets */

typedef enum STATE{
    START,
    CONNECTED,
    CONN_ACK,
    SUBSCRIBED,
    SUB_ACK,
    DISCONNECTED
}STATE;

typedef struct CONTEXT{
    MQTTpacket packet;
    struct sockaddr_in server;
    STATE state;
    int socket_fd;
    unsigned int BUFF_SIZE;
    char *BUFFER;
}CONTEXT;

CONTEXT *connectBroker(const char *host, unsigned int port, const char *clientID);
void subscribe(CONTEXT* ctx, const char *topic, unsigned char qos);
void publish(CONTEXT *ctx, const char *topic, const char *msg);
void disconnect(CONTEXT *ctx);

#endif
