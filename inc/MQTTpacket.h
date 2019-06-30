/*
TODO:
    Create remaining Control Headers,
    Add Control Codes,
    Implement remaining length function
    √
*/

#ifndef _MQTTpacket_H
#define _MQTTpacket_H

#include "MQTTcore.h"
#include <stdlib.h>
#include <string.h>

/*
 *==============================================================================
 *  	Implement Structures and function declarations
 *==============================================================================
 */

// Payload specific to MQTT-CONNECT message
typedef struct CONN_PAYLOAD{
	unsigned short   PROTO_NAME_LEN;
	unsigned char    PROTO_NAME[4];
	unsigned char    VERSION;
	unsigned char    CONN_FLAGS;
	unsigned short   KEEP_ALIVE;
	unsigned short   CLIENT_ID_LEN;
	char*   CLIENT_ID;
}CONN_PAYLOAD;

// DISCONNECT Packet has no payload only a header

// Payload specific to MQTT-SUBSCRIBE message
typedef struct SUB_PAYLOAD{
	unsigned short MSG_ID;
	unsigned short TOPIC_LEN;
	unsigned char QOS;
	char *TOPIC;
}SUB_PAYLOAD;


void MQTT_Connect(MQTTpacket *CP, const char *clientID, unsigned short keep_alive);
void MQTT_Subscribe(MQTTpacket *SP, const char *topic, unsigned char QOS);

#endif
