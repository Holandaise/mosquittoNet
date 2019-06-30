#include "MQTTi.h"

#define server_ip "192.168.0.101"
#define port 1883
#define clientID "mosq_001"
#define KEEPALIVE 30

int main(){
    CONTEXT *ctx = connectBroker(server_ip, port, clientID, KEEPALIVE);

	subscribe(ctx, "/test/switch", 0);

	while(ctx->state == SUBSCRIBED){

		ctx->BUFF_SIZE = recv(ctx->socket_fd, ctx->BUFFER, 512, 0);

		if(ctx->BUFF_SIZE == 0) break;

		printf("%d bytes recieved from broker\n", ctx->BUFF_SIZE);

		for(int i=0; i<ctx->BUFF_SIZE; i++){
			printf("%c", ctx->BUFFER[i]);
		}

		printf("\n");
	}

    disconnect(ctx);
    free(ctx);
    return 0;
}


