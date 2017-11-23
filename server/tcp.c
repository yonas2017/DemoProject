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
	if((p_tcp->m_listenSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("could not create socket.\n");
		return -1;
	}

	struct sockaddr_in server_address;
	memset((char *)&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(p_tcp->m_port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	// bind it to listen to the incoming connections on the created server address
	if(bind(p_tcp->m_listenSock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
	{
		printf("could not bind socket.\n");
		return -1;
	}

	int wait_size = 1;  // maximum number of waiting clients, after which dropping begins
	if(listen(p_tcp->m_listenSock, wait_size) < 0)
	{
		printf("could not open socket for listening.\n");
		return -1;
	}

	// socket address used to store client address
	struct sockaddr_in client_address;
	int client_address_len = 0;

	// open a new socket to transmit data per connection
	if ((p_tcp->m_newSock = accept(p_tcp->m_listenSock,
		(struct sockaddr*) &client_address,
		&client_address_len)) < 0)
		{
			printf("could not open a socket to accept data\n");
			return -1;
		}

		printf("Waiting for incoming connections...\n");

		return 0;
	}

	int tcpSend(TcpPtr p_tcp, const ServerMessagePtr p_message)
	{
		const unsigned int ServerMessageHeaderSize = sizeof(p_message->JobInfo) + sizeof(p_message->jobTextLength);

		if(send(p_tcp->m_newSock, p_message, p_message->jobTextLength + ServerMessageHeaderSize, 0) < 0)
		{
			printf("could not seng message to client\n");
			return 0;
		}

		return 1;
	}

	int tcpReceive(TcpPtr p_tcp, ClientMessagePtr p_message)
	{
		if(recv(p_tcp->m_newSock, p_message, sizeof(struct ClientMessage), 0) < 0)
		{
			printf("could not receive message from client.\n");
			return 0;
		}

		return 1;
	}

	void tcpClose(TcpPtr p_tcp)
	{
		// close the sockets
		close(p_tcp->m_listenSock);
		close(p_tcp->m_newSock);
	}
