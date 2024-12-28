#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "messages.h"
#include "tcp_server.h"

#pragma pack(1)  // Set 1-byte alignment

typedef struct
{
    tcp_t            tcp;
    client_message_t client_message;
    server_message_t server_message;
} server_t;

/*
 * client type message to string convertor.
 * @param client_message_type: client message type
 * @return: enum string translation
 */
const char* Client_message_type_str(enum ClientMessageType client_message_type);

/*
 * gets job from the server.
 * @param handler: file handler
 * @param file_size: total file size server opens for reading
 * @param total_read_size: total file size the server actually reads on request
 * @param client_message: client message
 * @param server_message: server message
 * @return: size of the file(job) read
 */
int get_job(FILE* handler, int long file_size, int long total_read_size,
           client_message_t* client_message, server_message_t* server_message);

/*
 * computes checksum.
 * @param job_text_length:length of the job text read by the server
 * @param job_tekst: job text read by the server
 * @return: the check sum
 */
uint8_t compute_checksum(unsigned int job_text_length, const char* job_tekst);

/*
 * fill in job info.
 * @param job_type:job type
 * @param checksum: computed check sum will be place in one byte.
 */
void set_job_info(unsigned char* job_type, uint8_t checksum);

/*
 * free resources used.
 * @param server: server
 * @param handler: file handler
 */
void release_resources(server_t* server, FILE* handler);

#endif
