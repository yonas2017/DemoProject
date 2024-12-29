#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "client.h"
#include "messages.h"
#include "pipe.h"
#include "tcp_client.h"

//#include "message_generated.h"

/*
 * Help Menu
 */
static void help()
{
    printf("./client <ip> <port>\n");
    printf("	<ip>   : Client IP Adress.\n");
    printf("	<port> : Client Port.\n");
}

// Create the client object
static client_t client;

static int pipeTermChildA[2];
static int pipeTermChildB[2];

/*
 * Define the function to be called when ctrl-c (SIGINT) signal is sent to
 * process
 */
static void sighandler(int sig)
{
    // Shutdown server
    client.client_message.msg_type = FAIL_TERMINATION;
    tcp_send(&client.tcp, &client.client_message);
    close_tcp_socket(&client);

    printf("Signal: %d caught... Shutting down Client.\n", sig);

    write_to_pipe(pipeTermChildA, 'Q', NULL);
    write_to_pipe(pipeTermChildB, 'Q', NULL);

    printf("\nWait Children to terminate....\n");
    wait_child();

    // exit client(parent) process
    printf("\nClient Exiting...\n");
    exit(-1);
}

/*
 * Main
 */
int main(int argc, char* argv[])
{
    printf("Client Started\n");

    // Register signal and signal handler
    // signal(SIGINT, &sighandler);

    struct sigaction sa = {0};
    sa.sa_handler = &sighandler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    // Check number of arguments passed by user.
    if (argc < 3)
    {
        help();
        exit(-1);
    }

    // Open TCP Socket
    client.tcp.ip = argv[1];
    sscanf(argv[2], "%hu", &client.tcp.port);

    // Creat Pipe for Client-to-Child communication
    int pipeChildA[2];
    int pipeChildB[2];
    create_pipe(pipeChildA, pipeChildB);
    create_pipe(pipeTermChildA, pipeTermChildB);

    // Fork childlen process to client
    pid_t fA = fork();
    if (fA > 0)
    {
        // Start Parent Process.
        parent_process(&client, pipeChildA, pipeChildB, pipeTermChildA,
                       pipeTermChildB);
    }
    else if (fA == 0)
    {
        pid_t fB = fork();
        if (fB > 0)
        {
            // Start Child Process A.
            child_process(pipeChildA, pipeTermChildA, STDOUT_FILENO);
        }
        else if (fB == 0)
        {
            // Start Child Process B.
            child_process(pipeChildB, pipeTermChildB, STDERR_FILENO);
        }
        else
        {
            printf("Error creating child process B.");
        }
    }
    else
    {
        printf("Error creating child process A.");
    }

    return 0;
}
