#include "MQTTi.h"

int main(){
    CONTEXT *ctx;
    ctx = connectBroker("192.168.0.101", 1883, "mosq_001");

    char *c = (ctx->state == CONNECTED) ? "Connected" : "Conn-Ack";

    printf("%s\n", c);
    getchar();
    disconnect(ctx);
    free(ctx);
    return 0;
}


