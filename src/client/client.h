#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include "messages.h"
#include "tcp_client.h"

#pragma pack(1)  // Set 1-byte alignment

typedef struct
{
    tcp_t            tcp;
    client_message_t client_message;
    server_message_t server_message;
} client_t;

/*
 * gets message from the user.
 * @param message : client message
 * @return: Error flag
 */
int get_client_message(client_message_t* message);

/*
 * gets jobtype specified by one byte.
 * @param jobInfo : jobInfo
 * @return: job type
 */
char get_job_type(const uint8_t* jobInfo);

/*
 * close Tcp Sockets.
 * @param client : TcpSocket
 */
void close_tcp_socket(client_t* client);

#endif
