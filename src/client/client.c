#include "client.h"
#include <stdio.h>
#include <stdlib.h>

int get_client_message(client_message_t* message)
{
    /*
     * Get input from User
     */
    if (!message)
    {
        printf("client message is null ptr.\n");
        return -1;
    }
    int option;
    printf("\n Choose 1 - 4 from the list below:\n\n");
    printf("1 - Get one Job from server.\n");
    printf("2 - Get X number of jobs from server\n");
    printf("3 - Get all jobs from server\n");
    printf("4 - Quit Program\n\n");

    int option_status = scanf("%u", &option);
    (void)option_status;

    switch (option)
    {
        case 1:
            printf("\n---------------------------------------------------.\n");
            printf("Getting %d job from the server has been selected.\n",
                   option);
            printf(
                "-----------------------------------------------------.\n\n");
            message->msg_type = GET_JOB;
            message->all_jobs = 0;
            message->num_jobs = 1;
            break;
        case 2:
            printf("\nEnter number of jobs to get from the server:\n");
            int num_job_status = scanf("%hu", &message->num_jobs);
            (void)num_job_status;

            printf("\n---------------------------------------------------.\n");
            printf("Getting %u job from the server has been selected.\n",
                   message->num_jobs);
            printf(
                "-----------------------------------------------------.\n\n");
            message->msg_type = GET_JOB;
            message->all_jobs = 0;
            break;
        case 3:
            printf("\n---------------------------------------------------.\n");
            printf("Getting all jobs from the server has been selected.\n");
            printf(
                "-----------------------------------------------------.\n\n");
            message->msg_type = GET_JOB;
            message->all_jobs = 1;
            message->num_jobs = 0;
            break;
        case 4:
            printf("\n---------------------------------------------------.\n");
            printf("Quiting program.\n");
            printf(
                "-----------------------------------------------------.\n\n");
            message->msg_type = QUIT_PROGRAM;
            message->all_jobs = 0;
            message->num_jobs = 0;
            break;
        default:
            printf("\n---------------------------------------------------.\n");
            printf("Invald choice  %d\n", option);
            printf(
                "-----------------------------------------------------.\n\n");
            message->msg_type = FAIL_TERMINATION;
            message->all_jobs = 0;
            message->num_jobs = 0;
            return -1;
    }

    return 0;
}

char get_job_type(const uint8_t* jobInfo)
{
    if (!jobInfo)
    {
        return 'X';
    }

    uint8_t jobType = (*jobInfo >> 5) & 0x07;
    switch (jobType)
    {
        case 0:
            return 'O';
        case 1:
            return 'E';
        case 7:
            return 'Q';
        default:
            return 'X';
    }
}

void close_tcp_socket(client_t* client)
{
    /*
     * Close TCP Sockets
     */
    if (client)
    {
        tcp_close(&client->tcp);
    }

    printf("TCP Socket Closed.\n");
}
