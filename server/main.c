#include "server.h"
#include "tcp.h"

#include <messages.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

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
static struct Server server;

/*
* Define the function to be called when ctrl-c (SIGINT) signal is sent to process
*/
static void sighandler(int sig)
{
	printf("Signal: %d caught... Shutting down Server.\n", sig);

	server.m_serverMessage.JobInfo = 'Q';           // JobType -> 'Q'
	server.m_serverMessage.jobTextLength = 0;       // jobTextLength = 0
	setJobInfo(&server.m_serverMessage.JobInfo, 0); // Checksun -> 0

	// Send job to client
	tcpSend(&server.m_tcp, &server.m_serverMessage);

	// Release Server Resources.
	releaseResources(&server, NULL);

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
	signal(SIGINT, &sighandler);

	// Check number of arguments passed by user.
	if(argc < 3)
	{
		help();
		exit(-1);
	}

	// Open TCP Socket
	server.m_tcp.m_fileName = argv[1];
	sscanf(argv[2], "%d", &server.m_tcp.m_port);
	if(tcpOpen(&server.m_tcp) != 0)
	{
		printf("Error! Opening Socket.\n");
		exit(-1);
	}

	// Open File for reading
	FILE* handler;
	if((handler = fopen(server.m_tcp.m_fileName, "rb")) == NULL)
	{
		printf("Error! Opening File.\n");
		releaseResources(&server, handler);
		exit(-1);
	}

	// Receive client message
	while(tcpReceive(&server.m_tcp, &server.m_clientMessage))
	{
		printf("received Client Message Type: '%s'\n", ClientMessageType_str((enum ClientMessageType) server.m_clientMessage.msg_type));

		if(server.m_clientMessage.msg_type == GET_JOB)
		{
			//Get Job file size
			long int readSize, fileSize;
			fseek(handler, 0L, SEEK_END);
			fileSize = ftell(handler);
			rewind(handler);

			//Read Job files and send it to the Client
			long int totalreadSize = 0;
			while((readSize = getJob(handler, fileSize, totalreadSize, &server.m_clientMessage, &server.m_serverMessage)) > 0)
			{
				//printf("JobType= %c\n", server.m_serverMessage.JobInfo);
				//printf("jobTextLength=%d\n", server.m_serverMessage.jobTextLength);
				//printf("%s\n", server.m_serverMessage.JobTekst);

				//Compute checksum
				uint8_t checksum = computeChecksum(server.m_serverMessage.jobTextLength, server.m_serverMessage.JobTekst);

				//Fill in Job Info
				setJobInfo(&server.m_serverMessage.JobInfo, checksum);

				//Send job to client
				tcpSend(&server.m_tcp, &server.m_serverMessage);

				//Advance the file pointer
				totalreadSize += readSize;
				fseek(handler, totalreadSize, SEEK_SET);

				// Delay for half a second for synchronizing the server to the client
				usleep(1000 * 500);
			}

			//Send to the client that the server has finished the Jobs
			if(totalreadSize == fileSize /*server.m_clientMessage.all_jobs*/)
			{
				printf("Finished Job. Sending Quit message to Client.\n");
				//Fill in 'Q' JobType
				server.m_serverMessage.JobInfo = 'Q';           // JobType -> 'Q'
				server.m_serverMessage.jobTextLength = 0;       // jobTextLength = 0
				setJobInfo(&server.m_serverMessage.JobInfo, 0); // Checksun -> 0

				//Send job to client
				tcpSend(&server.m_tcp, &server.m_serverMessage);
			}
		}
		else if(server.m_clientMessage.msg_type == NORMAL_TERMINATOIN)
		{
			printf("Received Client has terminated Normally, Exiting Server.\n");
			releaseResources(&server, handler);
			exit(0);
		}
		else if(server.m_clientMessage.msg_type == FAIL_TERMINATION)
		{
			printf("Received Client has terminated Abormally, Exiting Server.\n");
			releaseResources(&server, handler);
			exit(0);
		}
		else if(server.m_clientMessage.msg_type == QUIT_PROGRAM)
		{
			printf("Received Quit Message. Sending Quit message back to Client and Exiting Server.\n");
			//Fill in 'Q' JobType
			server.m_serverMessage.JobInfo = 'Q';           // JobType -> 'Q'
			server.m_serverMessage.jobTextLength = 0;       // jobTextLength = 0
			setJobInfo(&server.m_serverMessage.JobInfo, 0); // Checksun -> 0

			//Send job to client
			tcpSend(&server.m_tcp, &server.m_serverMessage);
			releaseResources(&server, handler);
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
	releaseResources(&server, handler);

	return 0;
}
