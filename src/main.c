#include "MQTTi.h"

int main(){
    CONTEXT *ctx = connectBroker("192.168.0.101", 1883, "mosq_001");
	subscribe(ctx, "/test/switch", 0);

	int x = recv(ctx->socket_fd, ctx->BUFFER, 17, MSG_WAITALL);

	printf("%d bytes recieved from broker\n", x);

    disconnect(ctx);
    free(ctx);
    return 0;
}


