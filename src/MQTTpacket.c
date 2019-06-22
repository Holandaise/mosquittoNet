#include "MQTTpacket.h"
#include <arpa/inet.h>

void MQTT_Connect(MQTTpacket *CP, const char *clientID)
{
	CP->payload = (CONN_PAYLOAD*)malloc(sizeof(CONN_PAYLOAD));
	CONN_PAYLOAD *PL = CP->payload;
	PL->CLIENT_ID = malloc(sizeof(char *));
	PL->CLIENT_ID_LEN = htons(strlen(clientID));
	strcpy(PL->CLIENT_ID, clientID);
	memcpy(PL->PROTO_NAME, "MQTT", 4);
	PL->VERSION = 4;
	PL->PROTO_NAME_LEN = htons(4);
	PL->KEEP_ALIVE = htons(40);
	PL->CONN_FLAGS = 2;
}

void MQTT_Subscribe(MQTTpacket *SP, const char* topic, char qos){
	static int ID = 1;
	SP->payload = (SUB_PAYLOAD*)malloc(sizeof(SUB_PAYLOAD));
	SUB_PAYLOAD *PL = SP->payload;
	PL->TOPIC_LEN = htons(strlen(topic));
	PL->TOPIC = malloc(sizeof(char*));
	PL->QOS = htons(SP->header.CNTL|=qos);
	PL->MSG_ID = htons(ID);
	strcpy(PL->TOPIC, topic);

	ID++;
}

static int calcLength(char *p, int x){
	char encoded;
	int i = 0;

	do{
		encoded = x % 128;
		x /= 128;

		if(x > 0){
			encoded |= 128;
		}

		p[i] = encoded;
		i++;
	}while(x > 0);

	return i;
}

/*
	Build MQTT connect message in BUFF
	Return number of bytes added to BUFF
	@param: pointer to BUFFER
	@param: pointer to MQTT Connect Packet
*/		
static int CONNECTbuilder(char *BUFF, MQTTpacket *self)
{
	CONN_PAYLOAD *PL = (CONN_PAYLOAD*)self->payload;
	int size = sizeof(CONN_PAYLOAD) - sizeof(char *) + strlen(PL->CLIENT_ID) - 4;

	int remLen = calcLength(
			self->header.remainingLen,
			size
			);

	memcpy(BUFF, &self->header, remLen + 1);//2 bytes
	memcpy(BUFF+remLen+1, &PL->PROTO_NAME_LEN, 2);
	memcpy(BUFF+remLen+3, PL->PROTO_NAME, 4);
	memcpy(BUFF+remLen+7, &PL->VERSION, 1);
	memcpy(BUFF+remLen+8, &PL->CONN_FLAGS, 1);
	memcpy(BUFF+remLen+9, &PL->KEEP_ALIVE, 2);
	memcpy(BUFF+remLen+11, &PL->CLIENT_ID_LEN, 2);
	strcpy(BUFF+remLen+13, PL->CLIENT_ID);

	free(PL);
	return size + (3-remLen);
}

/*
	Build MQTT subscribe message in BUFF
	Return number of bytes added to BUFF
	@param: pointer to BUFFER
	@param: pointer to MQTT Subscribe Packet
 */
static int SUBSCRIBEbuilder(char *BUFF, MQTTpacket *self)
{
	int bytesWritten;
	SUB_PAYLOAD *PL = (SUB_PAYLOAD*)self->payload;
	int remLen = calcLength(
		self->header.remainingLen,
		sizeof(SUB_PAYLOAD));
	
	memcpy(BUFF, &self->header, remLen+1);
	memcpy(BUFF+remLen+1, PL, sizeof(short)*2);
	strcpy(BUFF+remLen+1+(sizeof(short)*2), PL->TOPIC);

	//todo move QOS into buffer, and update retval
	bytesWritten = (remLen + 1 + sizeof(short)*2 + strlen(PL->TOPIC));
	free(self->payload);
	return bytesWritten;
}

const MQTTheader CONTROL_HEADERS[2] = {
    {0x10, {0,0,0,0}},
    {0x80, {0,0,0,0}}
};

MQTTpacket CONNECT_P = {
    CONTROL_HEADERS[CONNECT],
    NULL,
    CONNECTbuilder
};

MQTTpacket SUBSCRIBE_P = {
    CONTROL_HEADERS[SUBSCRIBE],
    NULL,
    SUBSCRIBEbuilder
};
