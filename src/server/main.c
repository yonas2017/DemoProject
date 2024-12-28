#include "messages.h"
#include "server.h"
#include "tcp_server.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Help Menu
 */
static void help()
{
    printf("./server <fileName> <port>\n");
    printf("	<fileName> : Job file to read from and send to cleant.\n");
    printf("	<port>     : Server Listening Port.\n");
}

// Create the Server object
static server_t server;

/*
 * Define the function to be called when ctrl-c (SIGINT) signal is sent to
 * process
 */
static void sighandler(int sig)
{
    printf("Signal: %d caught... Shutting down Server.\n", sig);

    server.server_message.job_info = 'Q';            // JobType -> 'Q'
    server.server_message.job_text_length = 0;        // jobTextLength = 0
    set_job_info(&server.server_message.job_info, 0);  // Checksun -> 0

    // Send job to client
    tcp_send(&server.tcp, &server.server_message);

    // Release Server Resources.
    release_resources(&server, NULL);

    // Shutdown server
    printf("\nServer Exiting...\n");
    exit(-1);
}

/*
 * Main
 */
int main(int argc, char* argv[])
{
    printf("Server Started\n");

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
    server.tcp.file_name = argv[1];
    //sscanf(argv[2], "%hu",
    //       &server.m_tcp.m_port);  // Read formatted input from a string
    server.tcp.port = (uint16_t) atoi(argv[2]);

    if (tcp_open(&server.tcp) != 0)
    {
        printf("Error! Opening Socket.\n");
        exit(-1);
    }

    // Open File for reading
    FILE* handler;
    if ((handler = fopen(server.tcp.file_name, "rb")) == NULL)
    {
        printf("Error! Opening File: %s.\n", server.tcp.file_name);
        release_resources(&server, handler);
        exit(-1);
    }

    // Receive client message
    while (tcp_receive(&server.tcp, &server.client_message))
    {
        printf("received Client Message Type: '%s'\n",
               Client_message_type_str(
                   (enum ClientMessageType)server.client_message.msg_type));

        if (server.client_message.msg_type == GET_JOB)
        {
            // Get Job file size
            long int readSize, fileSize;
            fseek(handler, 0L, SEEK_END);
            fileSize = ftell(handler);
            rewind(handler);

            // Read Job files and send it to the Client
            long int totalreadSize = 0;
            while ((readSize = get_job(handler, fileSize, totalreadSize,
                                      &server.client_message,
                                      &server.server_message)) > 0)
            {
                // printf("JobType= %c\n", server.m_serverMessage.JobInfo);
                // printf("jobTextLength=%d\n",
                // server.m_serverMessage.jobTextLength); printf("%s\n",
                // server.m_serverMessage.JobTekst);

                // Compute checksum
                uint8_t checksum =
                    compute_checksum(server.server_message.job_text_length,
                                    server.server_message.job_tekst);

                // Fill in Job Info
                set_job_info(&server.server_message.job_info, checksum);

                // Send job to client
                tcp_send(&server.tcp, &server.server_message);

                // Advance the file pointer
                totalreadSize += readSize;
                fseek(handler, totalreadSize, SEEK_SET);

                // Delay for half a second for synchronizing the server to the
                // client
                usleep(1000 * 500);
            }

            // Send to the client that the server has finished the Jobs
            if (totalreadSize == fileSize /*server.m_clientMessage.all_jobs*/)
            {
                printf("Finished Job. Sending Quit message to Client.\n");
                // Fill in 'Q' JobType
                server.server_message.job_info = 'Q';      // JobType -> 'Q'
                server.server_message.job_text_length = 0;  // jobTextLength = 0
                set_job_info(&server.server_message.job_info,
                           0);  // Checksun -> 0

                // Send job to client
                tcp_send(&server.tcp, &server.server_message);
            }
        }
        else if (server.client_message.msg_type == NORMAL_TERMINATOIN)
        {
            printf(
                "Received Client has terminated Normally, Exiting Server.\n");
            release_resources(&server, handler);
            exit(0);
        }
        else if (server.client_message.msg_type == FAIL_TERMINATION)
        {
            printf(
                "Received Client has terminated Abormally, Exiting Server.\n");
            release_resources(&server, handler);
            exit(0);
        }
        else if (server.client_message.msg_type == QUIT_PROGRAM)
        {
            printf(
                "Received Quit Message. Sending Quit message back to Client "
                "and Exiting Server.\n");
            // Fill in 'Q' JobType
            server.server_message.job_info = 'Q';      // JobType -> 'Q'
            server.server_message.job_text_length = 0;  // jobTextLength = 0
            set_job_info(&server.server_message.job_info, 0);  // Checksun -> 0

            // Send job to client
            tcp_send(&server.tcp, &server.server_message);
            release_resources(&server, handler);
            exit(0);
        }
        else
        {
            printf("Wrong Client Message Type.\n");
        }
    }

    /*
     * Release Server Resources.
     */
    release_resources(&server, handler);

    return 0;
}
