#include "MQTTi.h"

int main(){
    CONTEXT *ctx;
    ctx = connectBroker("192.168.0.101", 1883, "mosq_001");

    switch(ctx->state){
        case CONNECTED:
            printf("Connected\n");
            break;
        case CONN_ACK:
            printf("Connection Acknowledged\n");
            break;
        default:
            printf("Unknown error\n");
            break;
    }
    getchar();

    free(ctx);
    return 0;
}


