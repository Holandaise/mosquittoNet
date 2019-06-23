/*
Copyright: Adam Lewis Montgomery
Date: 06/21/2019

MQTTpacket is a library for the creation of MQTT message structures that
can be sent over the network to a broker. This library does not implement
any networking but simply forms the data into a network transferrable byte
array. It also can read a byte array from the network, and build appropriate
MQTT message structures for programming logic to handle.

*/

/*
TODO:
    Create remaining Control Headers,
    Add Control Codes,
    Implement remaining length function
    √
*/

#ifndef _MQTTpacket_H
#define _MQTTpacket_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
-----------------------------------------------------------------------------
    Basic Structures for building MQTT messages
-----------------------------------------------------------------------------
*/

// Index for MQTT header Control Codes
typedef const enum ControlCode{
    CONNECT,
    DISCONNECT,
    SUBSCRIBE
}ControlCode;

/*
    Generic MQTT header
    * 1 Byte for Control Code
    * Up to 4 Bytes for Remaining Length value
*/
typedef struct MQTTheader{
    char CNTL;
    char remainingLen[4];
}MQTTheader;


/*
    Generic MQTT message object
    Header is standard across all types of messages
    Payload differs based on message type
    build() points to a funtion to build the specific message type
*/
typedef struct MQTTpacket{
    MQTTheader header;
    void *payload;
    int (*build)(char *BUFF, struct MQTTpacket *self);
}MQTTpacket;

/*
-------------------------------------------------------------------------------
    Implemented structures and function declarations
-------------------------------------------------------------------------------
*/

// Payload specific to MQTT-CONNECT message
typedef struct CONN_PAYLOAD{
	short   PROTO_NAME_LEN;
	char    PROTO_NAME[4];
	char    VERSION;
	char    CONN_FLAGS;
	short   KEEP_ALIVE;
	short   CLIENT_ID_LEN;
	char*   CLIENT_ID;
}CONN_PAYLOAD;

// DISCONNECT Packet has no payload only a header

// Payload specific to MQTT-SUBSCRIBE message
typedef struct SUB_PAYLOAD{
	short MSG_ID;
	short TOPIC_LEN;
	char *TOPIC;
	char QOS;
}SUB_PAYLOAD;

const MQTTheader CONTROL_HEADERS[3]; // Array of pre-defined MQTT Headers indexed by Control Codes

extern MQTTpacket CONNECT_P;    // Packet defined in MQTTpacket.c
extern MQTTpacket SUBSCRIBE_P;  // Packet defined in MQTTpacket.c
extern MQTTpacket DISCONN_P;

void MQTT_Connect(MQTTpacket *CP, const char *clientID);
void MQTT_Subscribe(MQTTpacket *SP, const char *topic, char QOS);

int CONNECTbuilder(char *BUFF, MQTTpacket *self);

int SUBSCRIBEbuilder(char *BUFF, MQTTpacket *self);

int DISCONNECTbuilder(char *BUFF, MQTTpacket *self);
#endif
