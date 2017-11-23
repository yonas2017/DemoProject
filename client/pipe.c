#include "pipe.h"
#include "tcp.h"
#include "client.h"
#include <messages.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void createPipe(int* pipeA, int* pipeB)
{
  int p;
  p = pipe(pipeA);
  if(p < 0)
  {
    printf("Error Creating piple for Child A.");
    exit(1);
  }

  p= pipe(pipeB);
  if(p < 0)
  {
    printf("Error Creating piple for Child B.");
    exit(1);
  }

}
//
void childProcess(int* pipeFd, int* pipeTermFd, int fdOut)
{
  char readbuffer[1];
  int nBytes;

  /* Child process closes up write-side of pipe because it is not used by the child.*/
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

  while(1)
  {

    /* Child process reads from read-side of pipe, and prints to STDOUT/STDERR */

    //if( strncmp(readbuffer,"A", 1) == 0 )
    if( (nBytes=read(pipeTermFd[0], readbuffer, sizeof(readbuffer))) > 0 )
    {
      /*Close any opened pipe*/
      close(pipeFd[0]);
      close(pipeTermFd[0]);

      /*Wait anychild to terminate.*/
      waitChild();

      /*Terminate the child process.*/
      exit(NORMAL_EXIT_STATUS);
    }
    else if( (nBytes=read(pipeFd[0], readbuffer, sizeof(readbuffer))) > 0)
    {
      write(fdOut,readbuffer,nBytes);
    }
  }
}

void waitChild()
{
  /*Wait a child process to finish if any ....*/
  int childPid, childStatus;

  childPid=wait(&childStatus);

  if(childPid>0)
  {
    if(WEXITSTATUS(childStatus) == NORMAL_EXIT_STATUS)
    {
      printf("\nNormal Child termination.PID:%d, status:%d\n", childPid, WEXITSTATUS(childStatus));
    }
    else
    {
      printf("\nAbnormal Child termination.PID:%d, status:%d\n", childPid, WEXITSTATUS(childStatus));
    }
  }
}

void writeToPipe(int* pipeFd, char jobType, ServerMessagePtr msg)
{
  //printf("O.Length:%d\n",msg->jobTextLength);

  /* Parent process closes up read-side of pipe because it is not used by the parent. */
  close(pipeFd[0]);

  if(jobType == JOB_INFO_TYPE_Q)
  {
    char jobType = 'Q';
    write(pipeFd[1], &jobType, 1);
  }
  else
  {
    write(pipeFd[1], msg->JobTekst, msg->jobTextLength);
  }
}

void parentProcess(ClientPtr p_client, int* pipeChildA, int* pipeChildB,  int* pipeTermChildA, int* pipeTermChildB)
{
  if(tcpOpen(&p_client->m_tcp) != 0)
  {
    printf("Error! Opening Socket.\n");
    exit(-1);
  }

  // Get Client Message
  MENU:
  if(getClientMessage(&p_client->m_clientMessage) != 0)
  {
    printf("Error! Geting Client Message.\n");
    closeTcpSocket(p_client);
    exit(-1);
  }

  // Send client messsage to server
  tcpSend(&p_client->m_tcp, &p_client->m_clientMessage);

  // Receive server message
  while(tcpReceive(&p_client->m_tcp, &p_client->m_serverMessage))
  {
    char jobType = getJobType((uint8_t*)&p_client->m_serverMessage.JobInfo);

    if(jobType == JOB_INFO_TYPE_Q)
    {
      writeToPipe(pipeTermChildA, jobType, NULL);
      writeToPipe(pipeTermChildB, jobType, NULL);

      printf("\n\nWait Children to terminate....\n");
      close(pipeChildA[1]);
      close(pipeChildB[1]);
      close(pipeTermChildA[1]);
      close(pipeTermChildB[1]);
      waitChild();

      // send server that client has terminated Normally
      p_client->m_clientMessage.msg_type = NORMAL_TERMINATOIN;
      tcpSend(&p_client->m_tcp, &p_client->m_clientMessage);

      closeTcpSocket(p_client);

      // exit client(parent) process
      printf("\nClient Exiting Normally.\n");
      exit(0);
    }
    else if(jobType == JOB_INFO_TYPE_O)
    {
      writeToPipe(pipeChildA, jobType, &p_client->m_serverMessage);
    }
    else if(jobType == JOB_INFO_TYPE_E)
    {
      writeToPipe(pipeChildB, jobType, &p_client->m_serverMessage);
    }
    else
    {
      printf("\nWrong Job Type: %c\n", jobType);
    }

    // Delay for half a second for synchronizing the server to the client
    --p_client->m_clientMessage.num_jobs;
    if(!p_client->m_clientMessage.all_jobs &&
      p_client->m_clientMessage.num_jobs == 0)
      {
        usleep(1000 * 500);
        goto MENU;
      }
    }

    /*
    * Close Socket
    */
    closeTcpSocket(p_client);
  }
