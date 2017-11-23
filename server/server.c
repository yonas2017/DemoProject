#include "server.h"

const char* ClientMessageType_str(enum ClientMessageType clientMessageType)
{
  switch(clientMessageType)
  {
    case GET_JOB:            return "Get Job";
    case NORMAL_TERMINATOIN: return "Normal Termination";
    case FAIL_TERMINATION:   return "Fail Termination";
    case QUIT_PROGRAM:       return "Quit Program";
    default:                 return "Unknown";
  }
}

/*
* reads job type and job text length .
* @param handler: file handler pointer
* @param p_serverMessage: server message pointer
*/
static unsigned int readJobHeader(FILE* handler, ServerMessagePtr p_serverMessage)
{
  unsigned char buffer[8];
  unsigned int jobHeaderSize = sizeof(p_serverMessage->JobInfo) + sizeof(p_serverMessage->jobTextLength);

  if(handler)
  {
    // Read it all in one operation
    fread(buffer, sizeof(char), jobHeaderSize, handler);

    //
    p_serverMessage->JobInfo = *((unsigned char*) &buffer[0]);
    p_serverMessage->jobTextLength = *((unsigned int*) &buffer[1]);
  }
  else
  {
    printf("Error! NULL handler.\n");
  }

  return jobHeaderSize;
}

/*
* reads job texts.
* @param handler: file handler pointer
* @param totalreadSize: total size of message(jobs) read by the server
* @param p_serverMessage: server message pointer
* @return: returns text size read by server.
*/
static int readJobTekst(FILE* handler, int long totalreadSize, ServerMessagePtr p_serverMessage)
{
  unsigned int jobHeaderSize = readJobHeader(handler, p_serverMessage);

  int readSize = jobHeaderSize;
  if(handler)
  {
    // Seek to the beginning of the file
    fseek(handler, totalreadSize + jobHeaderSize, SEEK_SET);

    // Read it all in one operation
    readSize += fread(p_serverMessage->JobTekst, sizeof(char), p_serverMessage->jobTextLength, handler);
  }
  else
  {
    printf("Error! NULL handler.\n");
  }

  return readSize;
}

int getJob(FILE* handler, int long fileSize, int long totalreadSize, ClientMessagePtr p_clientMessage, ServerMessagePtr p_serverMessage)
{
  int readSize = 0;
  if(totalreadSize < fileSize)
  {
    if(p_clientMessage->num_jobs)
    {
      readSize = readJobTekst(handler, totalreadSize, p_serverMessage);
      --p_clientMessage->num_jobs;
    }
    else if(p_clientMessage->all_jobs)
    {
      readSize = readJobTekst(handler, totalreadSize, p_serverMessage);
    }
  }

  return readSize;
}

uint8_t computeChecksum(unsigned int jobTextLength, const char* JobTekst)
{
  const uint8_t* job = (const uint8_t*) JobTekst;
  unsigned int i;
  uint8_t checksum = 0;
  for(i=0; i < jobTextLength - 1; i++)
  {
    checksum += (job[i] % 32);
  }

  return checksum;
}

void setJobInfo(unsigned char* jobType, uint8_t checksum)
{
  uint8_t* jobInfo = (uint8_t*) jobType;
  switch(*jobType)
  {
    case 'O':
    *jobInfo = (0 << 5) | ((checksum >> 3) & 0x1F);
    break;
    case 'E':
    *jobInfo = (1 << 5) | ((checksum >> 3) & 0x1F);
    break;
    case 'Q':
    *jobInfo = (7 << 5) | ((checksum >> 3) & 0x1F);
    break;
    default:
    printf("Wrong Job Type.\n");
    break;
  }
}

void releaseResources(ServerPtr p_server, FILE* handler)
{
  /*
  * Close the file.
  */
  if(handler)
  {
    fclose(handler);
  }

  /*
  * Close TCP Sockets
  */
  if(p_server)
  {
    tcpClose(&p_server->m_tcp);
  }

  printf("Released Server Resources.\n");
}
