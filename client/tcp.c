#include "tcp.h"

#include <sys/types.h>  // For data types
#include <arpa/inet.h>  // For inet_addr()
#include <sys/socket.h> // For socket(), connect(), send(), and recv()
#include <netinet/in.h> // For sockaddr_in
#include <unistd.h>     // For close()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int tcpOpen(TcpPtr p_tcp)
{
	// open a stream socket
	if((p_tcp->m_sockDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("could not create socket.\n");
		return -1;
	}

	struct sockaddr_in server_address;
	memset((char *)&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(p_tcp->m_port);
	server_address.sin_addr.s_addr = inet_addr(p_tcp->m_ip);

	// TCP is connection oriented, a reliable connection must be established before any data is exchanged
	if(connect(p_tcp->m_sockDesc, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
	{
		printf("could not connect to server\n");
		return -1;
	}

	return 0;
}

int tcpSend(TcpPtr p_tcp, const ClientMessagePtr p_message)
{
	if(send(p_tcp->m_sockDesc, p_message, sizeof(struct ClientMessage), 0) < 0)
	{
		printf("could not seng message to server\n");
		return 0;
	}

	return 1;
}

int tcpReceive(TcpPtr p_tcp, ServerMessagePtr p_message)
{
	if(recv(p_tcp->m_sockDesc, p_message, sizeof(struct ServerMessage), 0) < 0)
	{
		printf("could not receive message from server.\n");
		return 0;
	}

	return 1;
}

void tcpClose(TcpPtr p_tcp)
{
	// close the socket
	close(p_tcp->m_sockDesc);
}
