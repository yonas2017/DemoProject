#ifndef TCP_H
#define TCP_H

#include <stdint.h>
#include <string.h>
#include "messages.h"

typedef struct
{
    char* file_name;  // File name holding one or more jobs used to send to the
                      // client
    uint16_t port;    // Server listening Port
    int      listen_sock;  // TCP socket
    int      new_sock;     // New socket to transmit data per connection
} tcp_t;

/*
 * open a stream Socket.
 * @param tcp: tcp socket
 * @return: Error flag
 */
int tcp_open(tcp_t* tcp);

/*
 * Send UDP message.
 * @param message: server message
 * @param tcp: tcp socket
 * @return: Error flag
 */
int tcp_send(tcp_t* tcp, const server_message_t* message);

/*
 * Receive UDP message.
 * @param message: client message
 * @param tcp: tcp socket
 * @return: Error flag
 */
int tcp_receive(tcp_t* tcp, client_message_t* message);

/*
 * close the TcpSocket.
 * @param tcp: tcp socket
 */
void tcp_close(tcp_t* tcp);

#endif
