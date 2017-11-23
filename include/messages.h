#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>

enum ClientMessageType
{
	GET_JOB=0,
	NORMAL_TERMINATOIN,
	FAIL_TERMINATION,
	QUIT_PROGRAM
};

/*
* Used for sending a message from Client to Server using tcp socket.
*/
struct __attribute__((__packed__)) ClientMessage
{
	uint8_t  msg_type;
	uint8_t  all_jobs;
	uint16_t num_jobs;
};

typedef struct ClientMessage* ClientMessagePtr;

/*
* Used for sending a message from Server to Client using tcp socket
*/
struct __attribute__((__packed__)) ServerMessage
{
	unsigned char JobInfo;
	unsigned int jobTextLength;
	char JobTekst[4096];
};

typedef struct ServerMessage* ServerMessagePtr;

#endif
