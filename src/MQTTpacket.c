#include "MQTTpacket.h"
#include <arpa/inet.h>

/* Static Variables */

// Definition of all MQTT control headers
static const MQTTheader CONTROL_HEADERS[] = {
	{0x10, {0,0,0,0}},
	{0xE0, {0,0,0,0}},
    {0x80, {0,0,0,0}}
};

/* Static Functions */

/* Calculate Remaining Length, and insert into field
 * @param: pointer to MQTT_packet
 * @param: size of the MQTT_packet
 * @return: number of bytes altered in the field
 */
static int calcLength(unsigned char *p, unsigned int x)
{
	unsigned char encoded;
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

/* Build MQTT connect message in BUFF
 * @param: pointer to BUFFER
 * @param: pointer to MQTT Connect Packet
 * @return: number of bytes written to BUFF
 */
static int CONNECTbuilder(char *BUFF, MQTTpacket *self)
{
	CONN_PAYLOAD *PL = (CONN_PAYLOAD*)self->payload;
	unsigned int size = 12 + strlen(PL->CLIENT_ID);

	unsigned int remLen = calcLength(
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

	free(PL->CLIENT_ID);
	free(PL);
	return size + remLen + 1;
}

/* Build MQTT subscribe message in BUFF
 * @param: pointer to BUFFER
 * @param: pointer to MQTT Subscribe Packet
 * @return: number of bytes written to BUFF
 */
static int SUBSCRIBEbuilder(char *BUFF, MQTTpacket *self)
{
	SUB_PAYLOAD *PL = (SUB_PAYLOAD *)self->payload;
	unsigned int size = 5 + strlen(PL->TOPIC);
	unsigned int remLen = calcLength(
		self->header.remainingLen,
		size);
	
	memcpy(BUFF, &self->header, remLen+1);
	memcpy(BUFF+remLen+1, PL, 4);
	strcpy(BUFF+remLen+5, PL->TOPIC);
	memcpy(BUFF+remLen+5+strlen(PL->TOPIC), &PL->QOS, 1);

	free(PL->TOPIC);
	free(self->payload);
	// 3-remLen = header size 
	return size + remLen + 1;
}

/* Build MQTT Disconnect message in BUFF
 * @param: pointer to BUFFER
 * @param: pointer to MQTT Disconnect Packet
 * @return: number of bytes written to BUFF
 */
static int DISCONNECTbuilder(char *BUFF, MQTTpacket *self)
{
	// Only Control Header and 1 byte of remaining length set to 0
	memcpy(BUFF, &self->header, 2);

	return 2;
}
/* Interface Variables */

// Concrete connect packet
MQTTpacket CONNECT_P = {
    CONTROL_HEADERS[CONNECT],
    NULL,
    CONNECTbuilder
};

// Concrete disconnect packet
MQTTpacket DISCONN_P = {
	CONTROL_HEADERS[DISCONNECT],
	NULL,
	DISCONNECTbuilder
};

// Concrete subscribe packet
MQTTpacket SUBSCRIBE_P = {
    CONTROL_HEADERS[SUBSCRIBE],
    NULL,
    SUBSCRIBEbuilder
};


/*	Interface Functions	*/

/* Build the connect packet
 * @param: pointer to CONNECT_P
 * @param: name for server to register
 */
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

/* Build the subscribe packet
 * @param: pointer to SUBSCRIBE_P
 * @param: topic name
 * @param: QOS 1-3
 */
void MQTT_Subscribe(MQTTpacket *SP, const char* topic, unsigned char qos)
{
	static unsigned int ID = 1;
	SP->payload = (SUB_PAYLOAD*)malloc(sizeof(SUB_PAYLOAD));
	SUB_PAYLOAD *PL = SP->payload;
	PL->TOPIC_LEN = htons(strlen(topic));
	PL->TOPIC = malloc(sizeof(char*));
	PL->QOS = htons(SP->header.CNTL|=qos);
	PL->MSG_ID = htons(ID);
	strcpy(PL->TOPIC, topic);

	ID++;
}
