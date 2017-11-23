#include "client.h"
#include <stdio.h>
#include <stdlib.h>

int getClientMessage(ClientMessagePtr p_message)
{
	/*
	* Get input from User
	*/
	int option;
	printf("\n Choose 1 - 4 from the list below:\n\n");
	printf("1 - Get one Job from server.\n");
	printf("2 - Get X number of jobs from server\n");
	printf("3 - Get all jobs from server\n");
	printf("4 - Quit Program\n\n");

	scanf("%u", &option);

	switch(option)
	{
		case 1:
		printf("\n---------------------------------------------------.\n");
		printf("Getting %d job from the server has been selected.\n", option);
		printf("-----------------------------------------------------.\n\n");
		p_message->msg_type = GET_JOB;
		p_message->all_jobs = 0;
		p_message->num_jobs = 1;
		break;
		case 2:
		printf("\nEnter number of jobs to get from the server:\n");
		scanf("%u", &p_message->num_jobs);

		printf("\n---------------------------------------------------.\n");
		printf("Getting %u job from the server has been selected.\n", p_message->num_jobs);
		printf("-----------------------------------------------------.\n\n");
		p_message->msg_type = GET_JOB;
		p_message->all_jobs = 0;
		break;
		case 3:
		printf("\n---------------------------------------------------.\n");
		printf("Getting all jobs from the server has been selected.\n");
		printf("-----------------------------------------------------.\n\n");
		p_message->msg_type = GET_JOB;
		p_message->all_jobs = 1;
		p_message->num_jobs = 0;
		break;
		case 4:
		printf("\n---------------------------------------------------.\n");
		printf("Quiting program.\n");
		printf("-----------------------------------------------------.\n\n");
		p_message->msg_type = QUIT_PROGRAM;
		p_message->all_jobs = 0;
		p_message->num_jobs = 0;
		break;
		default:
		printf("\n---------------------------------------------------.\n");
		printf("Invald choice  %d\n", option);
		printf("-----------------------------------------------------.\n\n");
		return -1;
	}

	return 0;
}

char getJobType(uint8_t* jobInfo)
{
	uint8_t jobType = (*jobInfo >> 5) & 0x07;
	switch(jobType)
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

void closeTcpSocket(ClientPtr p_client)
{
	/*
	* Close TCP Sockets
	*/
	tcpClose(&p_client->m_tcp);

	printf("TCP Socket Closed.\n");
}
