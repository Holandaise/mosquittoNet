#include "MQTTi.h"

int main(){
    CONTEXT *ctx = connectBroker("192.168.0.101", 1883, "mosq_001");

	subscribe(ctx, "/test/switch", 0);

	while(ctx->state == SUB_ACK){
		ctx->BUFF_SIZE = recv(ctx->socket_fd, ctx->BUFFER, 512, 0);

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


