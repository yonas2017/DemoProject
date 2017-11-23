#ifndef TCP_H
#define TCP_H

#include <messages.h>
#include <string.h>
#include <stdint.h>

struct Tcp
{
	char* m_ip;
	uint16_t m_port;
	int m_sockDesc; // Internal Socket descriptor
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
* @param p_message: client message pointer
*@param p_tcp: tcp socket pointer
* @return: Error flag
*/
int tcpSend(TcpPtr p_tcp, const ClientMessagePtr p_message);

/*
* Receive UDP message.
* @param p_message: server message pointer
*@param p_tcp: tcp socket pointer
* @return: Error flag
*/
int tcpReceive(TcpPtr p_tcp, ServerMessagePtr p_message);

/*
* close the TcpSocket.
*@param p_tcp: tcp socket pointer
*/
void tcpClose(TcpPtr p_tcp);

#endif
