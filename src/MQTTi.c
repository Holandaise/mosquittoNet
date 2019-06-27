#include "MQTTi.h"

// interface MQTT_Connect(), and MQTT_Subscribe functions
// with the network. Think one layer out in the onion...

/* Create a connection to the MQTT broker
 * @param: broker IP address
 * @param: port number (1883 for MQTT)
 * @param: client ID to pass to MQTTconnect()
 * @return: pointer to a state context
 */
CONTEXT *connectBroker(const char *host, unsigned int port, const char *clientID)
{
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
    CTX->server.sin_addr.s_addr = inet_addr(host);
    CTX->server.sin_family = AF_INET;
    CTX->server.sin_port = htons(port);

    if(connect(CTX->socket_fd, (struct sockaddr*)&CTX->server, sizeof(struct sockaddr_in))){
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

    if(CTX->BUFFER[0] == 0x20){
        CTX->state = CONN_ACK;
    }
    return CTX;
}


/* Subscribe to a topic
 * @param: Context received from connect()
 * @param: topic to subscribe
 * @param: QOS
 */
void subscribe(CONTEXT *ctx, const char *topic, unsigned char qos)
{
	// TODO:
	// may need to fork here to run separate subscribers
	// 
	ctx->packet = SUBSCRIBE_P;
	MQTT_Subscribe(&ctx->packet, topic, qos);
	ctx->BUFF_SIZE = ctx->packet.build(ctx->BUFFER, &ctx->packet);

	if(send(ctx->socket_fd, ctx->BUFFER, ctx->BUFF_SIZE, 0)<0){
		printf("Error: cannot subscribe to %s\n", topic);
		close(ctx->socket_fd);
	}
	ctx->state = SUBSCRIBED;
}


/* Disconnect from server, and free all memory used in the context
 * @param: pointer to the context
 */
void disconnect(CONTEXT *ctx){
    //free previous context
    //NOTE: unecessary because i'm smart, and free packet after previous
    // buffer is written *wink wink*
    //free(ctx->packet.payload);

    //send disconnect
    ctx->packet = DISCONN_P;
    ctx->BUFF_SIZE = ctx->packet.build(ctx->BUFFER, &ctx->packet);

    if(send(ctx->socket_fd, ctx->BUFFER, ctx->BUFF_SIZE, 0)<0){
        puts("Error: cannot send packet DISCONNECT\n");
        
        exit(EXIT_FAILURE);
    }
    //tear down context, free memory
    close(ctx->socket_fd);
    free(ctx->BUFFER);
    ctx->state = DISCONNECTED;
}