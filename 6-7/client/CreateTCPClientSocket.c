#ifndef CLIENT_CREATE_TCP_CLIENT

#define CLIENT_CREATE_TCP_CLIENT

#include "./Common.h"

int CreateTCPClientSocket(char *servIP, unsigned short servPort)
{
  int sock;                        /* socket to create */
  struct sockaddr_in echoServAddr; /* Local address */

  /* Create a reliable, stream socket using TCP */
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithError("socket() failed");

  /* Construct the server address structure */
  memset(&echoServAddr, 0, sizeof(echoServAddr));   /* Zero out structure */
  echoServAddr.sin_family = AF_INET;                /* Internet address family */
  echoServAddr.sin_addr.s_addr = inet_addr(servIP); /* Server IP address */
  echoServAddr.sin_port = htons(servPort);          /* Server port */

  /* Establish the connection to the echo server */
  if (connect(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) < 0)
    DieWithError("connect() failed");

  return sock;
}

#endif
