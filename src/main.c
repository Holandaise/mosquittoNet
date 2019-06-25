#include "MQTTi.h"

int main(){
    CONTEXT *ctx = connectBroker("192.168.0.101", 1883, "mosq_001");

    getchar();
    disconnect(ctx);
    free(ctx);
    return 0;
}


