#include "pipe.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>  // For wait()
#include <unistd.h>
#include "client.h"
#include "tcp_client.h"

void create_pipe(int* pipeA, int* pipeB)
{
    int p;
    p = pipe(pipeA);
    if (p < 0)
    {
        printf("Error Creating pipe for Child A.");
        exit(1);
    }

    p = pipe(pipeB);
    if (p < 0)
    {
        printf("Error Creating pipe for Child B.");
        exit(1);
    }
}

void child_process(int* pipeFd, int* pipeTermFd, int fdOut)
{
    char readbuffer[1];
    int  nBytes;

    /* Child process closes up write-side of pipe because it is not used by the
     * child.*/
    close(pipeFd[1]);
    close(pipeTermFd[1]);

    /* Do not block when reading from pipe.*/
    if (fcntl(pipeFd[0], F_SETFL, O_NONBLOCK) < 0)
    {
        exit(1);
    }

    if (fcntl(pipeTermFd[0], F_SETFL, O_NONBLOCK) < 0)
    {
        exit(1);
    }

    while (1)
    {
        /* Child process reads from read-side of pipe, and prints to
         * STDOUT/STDERR */

        // if( strncmp(readbuffer,"A", 1) == 0 )
        if ((nBytes = read(pipeTermFd[0], readbuffer, sizeof(readbuffer))) > 0)
        {
            /*Close any opened pipe*/
            close(pipeFd[0]);
            close(pipeTermFd[0]);

            /*Wait anychild to terminate.*/
            wait_child();

            /*Terminate the child process.*/
            exit(NORMAL_EXIT_STATUS);
        }
        else if ((nBytes = read(pipeFd[0], readbuffer, sizeof(readbuffer))) > 0)
        {
            ssize_t size = write(fdOut, readbuffer, nBytes);
            (void)size;
        }
    }
}

void wait_child()
{
    /*Wait a child process to finish if any ....*/
    int childPid, childStatus;

    childPid = wait(&childStatus);

    if (childPid > 0)
    {
        if (WEXITSTATUS(childStatus) == NORMAL_EXIT_STATUS)
        {
            printf("\nNormal Child termination. PID:%d, status:%d\n", childPid,
                   WEXITSTATUS(childStatus));
        }
        else
        {
            printf("\nAbnormal Child termination. PID:%d, status:%d\n",
                   childPid, WEXITSTATUS(childStatus));
        }
    }
}

void write_to_pipe(int* pipeFd, char jobType, server_message_t* msg)
{
    ssize_t size;
    // printf("O.Length:%d\n",msg->jobTextLength);

    /* Parent process closes up read-side of pipe because it is not used by the
     * parent. */
    close(pipeFd[0]);

    if (jobType == JOB_INFO_TYPE_Q)
    {
        // char jobType = 'Q';
        size = write(pipeFd[1], &jobType, 1);
    }
    else
    {
        size = write(pipeFd[1], msg->job_tekst, msg->job_text_length);
    }

    (void)size;
}

void parent_process(client_t* client, int* pipeChildA, int* pipeChildB,
                    int* pipeTermChildA, int* pipeTermChildB)
{
    if (tcp_open(&client->tcp) != 0)
    {
        printf("Error! Opening Socket.\n");
        exit(-1);
    }

// Get Client Message
MENU:
    if (get_client_message(&client->client_message) != 0)
    {
        printf("Error! Geting Client Message.\n");
        close_tcp_socket(client);
        exit(-1);
    }

    // Send client messsage to server
    tcp_send(&client->tcp, &client->client_message);

    // Receive server message
    while (tcp_receive(&client->tcp, &client->server_message))
    {
        char jobType = get_job_type((uint8_t*)&client->server_message.job_info);

        if (jobType == JOB_INFO_TYPE_Q)
        {
            write_to_pipe(pipeTermChildA, jobType, NULL);
            write_to_pipe(pipeTermChildB, jobType, NULL);

            close(pipeChildA[0]);
            close(pipeChildB[0]);
            close(pipeChildA[1]);
            close(pipeChildB[1]);
            close(pipeTermChildA[1]);
            close(pipeTermChildB[1]);

            printf("\n\nWait Children to terminate....\n");
            wait_child();

            // send server that client has terminated Normally
            client->client_message.msg_type = NORMAL_TERMINATOIN;
            tcp_send(&client->tcp, &client->client_message);

            close_tcp_socket(client);

            // exit client(parent) process
            printf("\nClient Exiting Normally.\n");
            exit(0);
        }
        else if (jobType == JOB_INFO_TYPE_O)
        {
            write_to_pipe(pipeChildA, jobType, &client->server_message);
        }
        else if (jobType == JOB_INFO_TYPE_E)
        {
            write_to_pipe(pipeChildB, jobType, &client->server_message);
        }
        else
        {
            printf("\nWrong Job Type: %c\n", jobType);
        }

        // Delay for half a second for synchronizing the server to the client
        --client->client_message.num_jobs;
        if (!client->client_message.all_jobs &&
            client->client_message.num_jobs == 0)
        {
            usleep(1000 * 500);
            goto MENU;
        }
    }

    /*
     * Close Socket
     */
    close_tcp_socket(client);
}
