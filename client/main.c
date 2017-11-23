#include "client.h"
#include "tcp.h"
#include "pipe.h"
#include <signal.h>
#include <messages.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
static struct Client client;

static int pipeTermChildA[2];
static int pipeTermChildB[2];

/*
* Define the function to be called when ctrl-c (SIGINT) signal is sent to process
*/
static void sighandler(int sig)
{
  //Shutdown server
  client.m_clientMessage.msg_type = FAIL_TERMINATION;
  tcpSend(&client.m_tcp, &client.m_clientMessage);
  closeTcpSocket(&client);

  printf("Signal: %d caught... Shutting down Client.\n", sig);

  writeToPipe(pipeTermChildA, 'Q', NULL);
  writeToPipe(pipeTermChildB, 'Q', NULL);

  printf("\nWait Children to terminate....\n");
  waitChild();

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
  signal(SIGINT, &sighandler);

  // Check number of arguments passed by user.
  if(argc < 3)
  {
    help();
    exit(-1);
  }

  // Open TCP Socket
  client.m_tcp.m_ip = argv[1];
  sscanf(argv[2], "%u", &client.m_tcp.m_port);

  // Creat Pipe for Client-to-Child communication
  int pipeChildA[2];
  int pipeChildB[2];
  createPipe(pipeChildA, pipeChildB);
  createPipe(pipeTermChildA, pipeTermChildB);

  // Fork childlen process to client
  int fA = fork();
  if(fA > 0)
  {
    // Start Parent Process.
    parentProcess(&client, pipeChildA, pipeChildB, pipeTermChildA, pipeTermChildB);
  }
  else if(fA == 0)
  {
    int fB = fork();
    if(fB > 0)
    {
      // Start Child Process A.
      childProcess(pipeChildA, pipeTermChildA, STDOUT_FILENO);
    }
    else if(fB== 0)
    {
      // Start Child Process B.
      childProcess(pipeChildB, pipeTermChildB, STDERR_FILENO);
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
