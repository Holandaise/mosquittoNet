#include "MQTTi.h"
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// interface MQTT_Connect(), and MQTT_Subscribe functions
// with the network. Think one layer out in the onion...

CONTEXT *connectBroker(
    const char* host,
    int port,
    const char* clientID
){
    struct sockaddr_in server;
    //establish connection
    CONTEXT *CTX = malloc(sizeof(CONTEXT));
    CTX->BUFFER = malloc(sizeof(MQTTpacket));
    CTX->state = START; //if fail state won't change to connected
    CTX->packet = CONNECT_P;
    MQTT_Connect(&CTX->packet, clientID);
    CTX->BUFF_SIZE = CTX->packet.build(CTX->BUFFER, &CTX->packet);
    CTX->socket_fd = socket(AF_INET, SOCK_STREAM,0);
    if(CTX->socket_fd < 0){
        CTX->state = START;
        return CTX;
    }
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if(connect(CTX->socket_fd, (struct sockaddr*)&server, sizeof(server))){
        puts("Unable to connect\nExiting...\n");
        close(CTX->socket_fd);
        return CTX;
    }
    if(send(CTX->socket_fd, CTX->BUFFER, CTX->BUFF_SIZE, 0)<0){
        puts("Error: cannot send packet\n");
        close(CTX->socket_fd);
        return CTX;
    }
    CTX->state = CONNECTED;

    CTX->BUFF_SIZE = recv(CTX->socket_fd, CTX->BUFFER, CTX->BUFF_SIZE,0);
    if(CTX->BUFF_SIZE < 0){
        puts("Connection not acknowledged");
        close(CTX->socket_fd);
        CTX->state = START;
        return CTX;
    }
    for(int i=0; i<CTX->BUFF_SIZE; i++){
        printf("%02X ", CTX->BUFFER[i]);
    }
    if(CTX->BUFFER[0] == 0x20){
        CTX->state = CONN_ACK;
    }
    return CTX;
}

void subscribe(CONTEXT *ctx, const char *topic, char qos);