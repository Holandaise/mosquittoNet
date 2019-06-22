#ifndef _MQTTi_H
#define _MQTTi_H

#include "MQTTpacket.h"
#include <sys/socket.h>

/*  Interface to MQTTpackets and internet sockets */

typedef enum STATE{
    START,
    CONNECTED,
    CONN_ACK,
    SUBSCRIBED,
    SUB_ACK
}STATE;

typedef struct CONTEXT{
    MQTTpacket packet;
    STATE state;
    int socket_fd;
    int BUFF_SIZE;
    char *BUFFER;
}CONTEXT;

CONTEXT *connectBroker(const char *host, int port, const char *clientID);
void subscribe(CONTEXT* ctx, const char *topic, char qos);
void publish(CONTEXT *ctx, const char *topic, const char *msg);

#endif