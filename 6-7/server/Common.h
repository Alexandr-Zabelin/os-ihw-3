#ifndef SERVER_COMMON_H

#define SERVER_COMMON_H

#include <stdio.h>      /* for printf() */
#include <sys/socket.h> /* for accept() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <string.h>     /* for memset() */
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAXPENDING 5 /* Maximum outstanding connection requests */

void DieWithError(char *errorMessage); /* Error handling function */
int AcceptTCPConnection(int servSock);
void HandleTCPClient(int clntSocket);
int CreateTCPServerSocket(unsigned short port);

#endif