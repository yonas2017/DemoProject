#include "client.h"
#include <messages.h>
#include <stdbool.h>

// Defines for message types.
#define  JOB_INFO_TYPE_O 'O'
#define  JOB_INFO_TYPE_E 'E'
#define  JOB_INFO_TYPE_Q 'Q'

#define  NORMAL_EXIT_STATUS 100

/*
* Create pipe for Child A and B.
*@param pipeA: pipe pointer A
*@param pipeB: pipe pointer B
*/
void createPipe(int* pipeA, int* pipeB);

/*
* Connects to Child pipe,  then it reads data from pipe..
*@param pipeFd: pipe file descriptor pointer
*@param pipeTermFd: pipe terminate file descriptor pointer
*@param fdOut: STDOUT/STDERR file descriptor
*/
void childProcess(int* pipeFd, int* pipeTermFd, int fdOut);

/*
* Connects to Child pipe, and then writes to the pipe.
*@param pipeFd: pipe file descriptor pointer
*@param jobType: job type.
*@param msg: server message pointer
*/
void writeToPipe(int* pipeFd, char jobType, ServerMessagePtr msg);

/*
* Function that processes server data and assign text to
* the correct child.
*@param p_client: client pointer
*@param pipeChildA: pipeChildA pointer for STD_OUT
*@param pipeChildB: pipeChildA pointer for STD_ERROR
*@param pipeTermChildA: pipe for terminating child A pointer
*@param pipeTermChildB: pipe for terminating child B pointer
*/
void parentProcess(ClientPtr p_client, int* pipeChildA, int* pipeChildB,  int* pipeTermChildA, int* pipeTermChildB);

/*
* Wait and terminate child.
*/
void waitChild();
