#include "server.h"

const char* Client_message_type_str(enum ClientMessageType client_message_type)
{
    switch (client_message_type)
    {
        case GET_JOB:
            return "Get Job";
        case NORMAL_TERMINATOIN:
            return "Normal Termination";
        case FAIL_TERMINATION:
            return "Fail Termination";
        case QUIT_PROGRAM:
            return "Quit Program";
        default:
            return "Unknown";
    }
}

static unsigned int read_job_header(FILE*             handler,
                                    server_message_t* server_message)
{
    unsigned char buffer[8];
    unsigned int  job_header_size = sizeof(server_message->job_info) +
                                   sizeof(server_message->job_text_length);

    if (handler)
    {
        // Read it all in one operation
        int size = fread(buffer, sizeof(char), job_header_size, handler);
        (void)size;

        //
        server_message->job_info = *((unsigned char*)&buffer[0]);
        server_message->job_text_length = *((unsigned int*)&buffer[1]);
    }
    else
    {
        printf("Error! NULL handler.\n");
    }

    return job_header_size;
}

static int read_job_tekst(FILE* handler, int long total_read_size,
                          server_message_t* server_message)
{
    unsigned int job_header_size = read_job_header(handler, server_message);
    int          read_size = job_header_size;
    if (handler)
    {
        // Seek to the beginning of the file
        fseek(handler, total_read_size + job_header_size, SEEK_SET);

        // Read it all in one operation
        read_size += fread(server_message->job_tekst, sizeof(char),
                           server_message->job_text_length, handler);
    }
    else
    {
        printf("Error! NULL handler.\n");
    }

    return read_size;
}

int get_job(FILE* handler, int long file_size, int long total_read_size,
            client_message_t* client_message, server_message_t* server_message)
{
    int readSize = 0;
    if (total_read_size < file_size)
    {
        if (client_message->num_jobs)
        {
            readSize = read_job_tekst(handler, total_read_size, server_message);
            --client_message->num_jobs;
        }
        else if (client_message->all_jobs)
        {
            readSize = read_job_tekst(handler, total_read_size, server_message);
        }
    }

    return readSize;
}

uint8_t compute_checksum(unsigned int job_text_length, const char* job_tekst)
{
    const uint8_t* job = (const uint8_t*)job_tekst;
    unsigned int   i;
    uint8_t        checksum = 0;
    for (i = 0; i < job_text_length - 1; i++)
    {
        checksum += (job[i] % 32);
    }

    return checksum;
}

void set_job_info(unsigned char* job_type, uint8_t checksum)
{
    uint8_t* job_info = (uint8_t*)job_type;
    switch (*job_type)
    {
        case 'O':
            *job_info = (0 << 5) | ((checksum >> 3) & 0x1F);
            break;
        case 'E':
            *job_info = (1 << 5) | ((checksum >> 3) & 0x1F);
            break;
        case 'Q':
            *job_info = (7 << 5) | ((checksum >> 3) & 0x1F);
            break;
        default:
            printf("Wrong Job Type.\n");
            break;
    }
}

void release_resources(server_t* server, FILE* handler)
{
    /*
     * Close the file.
     */
    if (handler)
    {
        fclose(handler);
    }

    /*
     * Close TCP Sockets
     */
    if (server)
    {
        tcp_close(&server->tcp);
    }

    printf("Released Server Resources.\n");
}
