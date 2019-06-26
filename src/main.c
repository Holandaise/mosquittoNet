#include "MQTTi.h"

int main(){
    CONTEXT *ctx = connectBroker("192.168.0.101", 1883, "mosq_001");
	getchar();
	subscribe(ctx, "/test/switch", 0);
	getchar();
	if(ctx->state == SUBSCRIBED){
		printf("Halelujah\n");
	}
    disconnect(ctx);
    free(ctx);
    return 0;
}


