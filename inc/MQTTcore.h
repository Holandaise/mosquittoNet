#ifndef _MQTTcore_H
#define _MQTTcore_H
/*
 * ============================================================================
 * 	Basic Structures for building MQTT messages
 * ============================================================================
 */

// Index for MQTT header Control Codes
typedef enum ControlCode{
    CONNECT = 0,
    DISCONNECT,
    SUBSCRIBE
}ControlCode;


typedef struct MQTTheader{
    unsigned char CNTL;
    unsigned char remainingLen[4];
}MQTTheader;


/*
    Generic MQTT message object
    Header is standard across all types of messages
    Payload differs based on message type
    build() points to a function to build the specific message type
*/
typedef struct MQTTpacket{
    MQTTheader header;
    void *payload;
    int (*build)(unsigned char *BUFF, struct MQTTpacket *self);
}MQTTpacket;

extern MQTTpacket CONNECT_P;    // Packet defined in MQTTpacket.c
extern MQTTpacket SUBSCRIBE_P;  // Packet defined in MQTTpacket.c
extern MQTTpacket DISCONN_P;	// Packet defined in MQTTpacket.c

#endif
