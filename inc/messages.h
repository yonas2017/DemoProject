#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>

#pragma pack(1)  // Set 1-byte alignment

enum ClientMessageType
{
    GET_JOB = 0,
    NORMAL_TERMINATOIN,
    FAIL_TERMINATION,
    QUIT_PROGRAM
};

/*
 * Used for sending a message from Client to Server using tcp socket.
 */
typedef struct
{
    uint8_t  msg_type;
    uint8_t  all_jobs;
    uint16_t num_jobs;
} client_message_t;

/*
 * Used for sending a message from Server to Client using tcp socket
 */
typedef struct
{
    unsigned char job_info;
    unsigned int  job_text_length;
    char          job_tekst[4096];
} server_message_t;

#endif
