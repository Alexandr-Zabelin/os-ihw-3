#ifndef CLIENT_COMMON_H

#define CLIENT_COMMON_H

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>

#define RCVBUFSIZE 32
#define SEND_BUFFER_SIZE 32
#define MAX_SLEEP_SECONDS 5
#define ARTS_COUNT 5
#define MAX_VIEWERS_COUNT 5
#define ART_NAME_LENGTH 15
#define VISITOR_NAME_LENGTH 15

struct Art
{
  int viewers_count;
  char *name;
  sem_t *semaphore_ptr;
};

struct Art arts[ARTS_COUNT];

int CreateTCPClientSocket(char *servIP, unsigned short servPort);
void DieWithError(char *errorMessage);
void swap(int *a, int *b);
void *chill_in_gallery(void *arg);

#endif
