#ifndef SERVER_H
#define SERVER_H

#include "tcp.h"
#include <messages.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct Server
{
	struct Tcp    m_tcp;
	struct ClientMessage m_clientMessage;
	struct ServerMessage m_serverMessage;
};

typedef struct Server* ServerPtr;

/*
* client type message to string convertor.
* @param clientMessageType: client message type pointer
* @return: enum string translation
*/
const char* ClientMessageType_str(enum ClientMessageType clientMessageType);

/*
* gets job from the server.
* @param handler: file handler
* @param fileSize: total file size server opens for reading
* @param totalreadSize: total file size the server actually reads on request
* @param p_clientMessage: client message pointer
* @param p_serverMessage: server message pointer
* @return: size of the file(job) read
*/
int getJob(FILE* handler, int long fileSize, int long totalreadSize, ClientMessagePtr p_clientMessage, ServerMessagePtr p_serverMessage);

/*
* computes checksum.
* @param jobTextLength:length of the job text read by the server
* @param JobTekst: job text read by the server pointer
* @return: the check sum
*/
uint8_t computeChecksum(unsigned int jobTextLength, const char* JobTekst);

/*
* fill in job info.
* @param jobType:job type pointer
* @param checksum: computed check sum will be place in one byte.
*/
void setJobInfo(unsigned char* jobType, uint8_t checksum);

/*
* free resources used.
* @param p_server: server pointer
* @param handler: file handler pointer
*/
void releaseResources(ServerPtr p_server, FILE* handler);

#endif
