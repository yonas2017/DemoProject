#ifndef TCP_H
#define TCP_H

#include <messages.h>
#include <string.h>
#include <stdint.h>

struct Tcp
{
	char* m_fileName; // Filename holding one or more jobs used to send to the client
	uint16_t m_port;  // Server listening Port
	int m_listenSock; // TCP socket
	int m_newSock ;   // New socket to transmit data per connection
};

typedef struct Tcp* TcpPtr;

/*
* open a stream Socket.
* @param p_tcp: tcp socket pointer
* @return: Error flag
*/
int tcpOpen(TcpPtr p_tcp);

/*
* Send UDP message.
* @param p_message: server message pointer
*@param p_tcp: tcp socket pointer
* @return: Error flag
*/
int tcpSend(TcpPtr p_tcp, const ServerMessagePtr p_message);

/*
* Receive UDP message.
* @param p_message: client message pointer
*@param p_tcp: tcp socket pointer
* @return: Error flag
*/
int tcpReceive(TcpPtr p_tcp, ClientMessagePtr p_message);

/*
* close the TcpSocket.
*@param p_tcp: tcp socket pointer
*/
void tcpClose(TcpPtr p_tcp);

#endif
