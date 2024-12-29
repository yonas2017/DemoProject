#include "client.h"
#include "messages.h"

// Defines for message types.
#define JOB_INFO_TYPE_O 'O'
#define JOB_INFO_TYPE_E 'E'
#define JOB_INFO_TYPE_Q 'Q'

#define NORMAL_EXIT_STATUS 100

/*
 * Create pipe for Child A and B.
 *@param pipeA: pipe pointer A
 *@param pipeB: pipe pointer B
 */
void create_pipe(int* pipeA, int* pipeB);

/*
 * Connects to Child pipe,  then it reads data from pipe..
 *@param pipeFd: pipe file descriptor pointer
 *@param pipeTermFd: pipe terminate file descriptor pointer
 *@param fdOut: STDOUT/STDERR file descriptor
 */
void child_process(int* pipeFd, int* pipeTermFd, int fdOut);

/*
 * Connects to Child pipe, and then writes to the pipe.
 *@param pipeFd: pipe file descriptor pointer
 *@param jobType: job type.
 *@param msg: server message pointer
 */
void write_to_pipe(int* pipeFd, char jobType, server_message_t* msg);

/*
 * Function that processes server data and assign text to
 * the correct child.
 *@param client: client
 *@param pipeChildA: pipeChildA pointer for STD_OUT
 *@param pipeChildB: pipeChildA pointer for STD_ERROR
 *@param pipeTermChildA: pipe for terminating child A pointer
 *@param pipeTermChildB: pipe for terminating child B pointer
 */
void parent_process(client_t* client, int* pipeChildA, int* pipeChildB,
                    int* pipeTermChildA, int* pipeTermChildB);

/*
 * Wait and terminate child.
 */
void wait_child();
