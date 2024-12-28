#ifndef TCP_H
#define TCP_H

#include <stdint.h>
#include <string.h>
#include "messages.h"

typedef struct
{
    char*    ip;
    uint16_t port;
    int      sockDesc;  // Internal Socket descriptor
} tcp_t;

/*
 * open a stream Socket.
 * @param tcp: tcp socket
 * @return: Error flag
 */
int tcp_open(tcp_t* tcp);

/*
 * Send UDP message.
 * @param message: client message
 * @param tcp: tcp socket
 * @return: Error flag
 */
int tcp_send(tcp_t* tcp, const client_message_t* message);

/*
 * Receive UDP message.
 * @param message: server message
 * @param tcp: tcp socket
 * @return: Error flag
 */
int tcp_receive(tcp_t* tcp, server_message_t* message);

/*
 * close the TcpSocket.
 * @param tcp: tcp socket
 */
void tcp_close(tcp_t* tcp);

#endif
