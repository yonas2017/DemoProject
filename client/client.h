#ifndef CLIENT_H
#define CLIENT_H

#include "tcp.h"
#include <messages.h>
#include <stdint.h>

struct Client
{
	struct Tcp    m_tcp;
	struct ClientMessage m_clientMessage;
	struct ServerMessage m_serverMessage;
};

typedef struct Client* ClientPtr;

/*
* gets message from the user.
* @param p_message : client message pointer
* @return: Error flag
*/
int getClientMessage(ClientMessagePtr p_message);

/*
* gets jobtype specified by one byte.
* @param JobType : jobType pointer
* @return: job type
*/
char getJobType(uint8_t* JobType);

/*
* close Tcp Sockets.
* @param p_client : TcpSocket pointer.
*/
void closeTcpSocket(ClientPtr p_client);

#endif
