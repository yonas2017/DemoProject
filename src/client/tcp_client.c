#include "tcp_client.h"

#include <arpa/inet.h>   // For inet_addr()
#include <netinet/in.h>  // For sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>  // For socket(), connect(), send(), and recv()
#include <sys/types.h>   // For data types
#include <unistd.h>      // For close()

int tcp_open(tcp_t* tcp)
{
    // open a stream socket
    if ((tcp->sockDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("could not create socket.\n");
        return -1;
    }

    struct sockaddr_in server_address;
    memset((char *)&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(tcp->port);
    server_address.sin_addr.s_addr = inet_addr(tcp->ip);

    // TCP is connection oriented, a reliable connection must be established
    // before any data is exchanged
    if (connect(tcp->sockDesc, (struct sockaddr *)&server_address,
                sizeof(server_address)) < 0)
    {
        printf("could not connect to server\n");
        return -1;
    }

    return 0;
}

int tcp_send(tcp_t* tcp, const client_message_t* message)
{
    if (send(tcp->sockDesc, message, sizeof(client_message_t), 0) < 0)
    {
        printf("could not seng message to server\n");
        return 0;
    }

    return 1;
}

int tcp_receive(tcp_t* tcp, server_message_t* message)
{
    if (recv(tcp->sockDesc, message, sizeof(server_message_t), 0) < 0)
    {
        printf("could not receive message from server.\n");
        return 0;
    }

    return 1;
}

void tcp_close(tcp_t* tcp)
{
    // close the socket
    close(tcp->sockDesc);
}
