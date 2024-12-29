#include "tcp_server.h"

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
    if ((tcp->listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("could not create socket.\n");
        return -1;
    }

    struct sockaddr_in server_address;
    memset((char*)&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(tcp->port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind it to listen to the incoming connections on the created server
    // address
    if (bind(tcp->listen_sock, (struct sockaddr*)&server_address,
             sizeof(server_address)) < 0)
    {
        printf("could not bind socket.\n");
        return -1;
    }

    int wait_size =
        1;  // maximum number of waiting clients, after which dropping begins
    if (listen(tcp->listen_sock, wait_size) < 0)
    {
        printf("could not open socket for listening.\n");
        return -1;
    }

    // socket address used to store client address
    struct sockaddr_in client_address;
    unsigned int       client_address_len = 0;

    // open a new socket to transmit data per connection
    if ((tcp->new_sock =
             accept(tcp->listen_sock, (struct sockaddr*)&client_address,
                    &client_address_len)) < 0)
    {
        printf("could not open a socket to accept data\n");
        return -1;
    }

    printf("Waiting for incoming connections...\n");

    return 0;
}

int tcp_send(tcp_t* tcp, const server_message_t* message)
{
    const unsigned int ServerMessageHeaderSize =
        sizeof(message->job_info) + sizeof(message->job_text_length);

    if (send(tcp->new_sock, message,
             message->job_text_length + ServerMessageHeaderSize, 0) < 0)
    {
        printf("could not seng message to client\n");
        return 0;
    }

    return 1;
}

int tcp_receive(tcp_t* tcp, client_message_t* message)
{
    if (recv(tcp->new_sock, message, sizeof(client_message_t), 0) < 0)
    {
        printf("could not receive message from client.\n");
        return 0;
    }

    return 1;
}

void tcp_close(tcp_t* tcp)
{
    // close the sockets
    close(tcp->listen_sock);
    close(tcp->new_sock);
}
