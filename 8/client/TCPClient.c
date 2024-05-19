#ifndef CLIENT_TCP_CLIENT

#define CLIENT_TCP_CLIENT

#include "./Common.h"

void clearBuffer(char *str, int str_len)
{
  memset(str, 0, str_len);
}

int main(int argc, char *argv[])
{
  int sock;
  unsigned short servPort;
  char *servIP;
  char sendBuffer[SEND_BUFFER_SIZE];
  int sentMessageLen;
  char rcvBuffer[RCVBUFSIZE];
  int bytesRcvd;
  int numberOfVisitors;

  if (argc < 5)
  {
    fprintf(stderr, "Usage: %s <Server IP> <Port> <Gallery name> <Number of visitors>\n", argv[0]);
    exit(1);
  }

  servIP = argv[1];
  servPort = atoi(argv[2]);
  numberOfVisitors = atoi(argv[4]);

  char **arts_names = (char **)malloc(sizeof(char *) * ARTS_COUNT);

  pid_t pid = getpid();

  for (int i = 0; i < ARTS_COUNT; ++i)
  {
    sem_t *semaphore_ptr;

    arts_names[i] = (char *)malloc(sizeof(char) * ART_NAME_LENGTH);
    snprintf(arts_names[i], ART_NAME_LENGTH, "Art %ld", pid + (long)i);

    sem_unlink(arts_names[i]);

    if ((semaphore_ptr = sem_open(arts_names[i], O_CREAT | O_EXCL, 0666, MAX_VIEWERS_COUNT)) == SEM_FAILED)
    {
      perror("Failed to open semaphore. Please try again\n");
      exit(1);
    }

    arts[i] = (struct Art){.viewers_count = 0, .name = arts_names[i], .semaphore_ptr = semaphore_ptr};
  }

  char **visitors_names = (char **)malloc(sizeof(char *) * numberOfVisitors);

  for (int i = 0; i < numberOfVisitors; ++i)
  {
    visitors_names[i] = (char *)malloc(sizeof(char) * VISITOR_NAME_LENGTH);
    snprintf(visitors_names[i], VISITOR_NAME_LENGTH, "visitor %d", i);
  }

  sock = CreateTCPClientSocket(servIP, servPort);

  snprintf(sendBuffer, SEND_BUFFER_SIZE, "OPEN %s", argv[3]);
  sentMessageLen = strlen(sendBuffer);

  if (send(sock, sendBuffer, sentMessageLen, 0) != sentMessageLen)
  {
    DieWithError("send() sent a different number of bytes than expected");
  }

  if ((bytesRcvd = recv(sock, rcvBuffer, RCVBUFSIZE - 1, 0)) <= 0)
  {
    DieWithError("recv() failed or connection closed prematurely");
  }

  rcvBuffer[bytesRcvd] = '\0';

  int currentVisitorIdx = 0;

  while (currentVisitorIdx < numberOfVisitors)
  {
    snprintf(sendBuffer, SEND_BUFFER_SIZE, "ENTER %s", visitors_names[currentVisitorIdx]);
    sentMessageLen = strlen(sendBuffer);

    if (send(sock, sendBuffer, sentMessageLen, 0) != sentMessageLen)
    {
      DieWithError("send() sent a different number of bytes than expected");
    }

    if ((bytesRcvd = recv(sock, rcvBuffer, RCVBUFSIZE - 1, 0)) <= 0)
    {
      DieWithError("recv() failed or connection closed prematurely");
    }

    rcvBuffer[bytesRcvd] = '\0';

    if (rcvBuffer[0] == 'Y')
    {
      printf("Visitor %s enters\n", visitors_names[currentVisitorIdx]);
      clearBuffer(rcvBuffer, RCVBUFSIZE);
      ++currentVisitorIdx;
      pid_t pid = fork();

      if (pid == -1)
      {
        perror("Failed to fork process. Please try again\n");
        exit(1);
      }
      else if (pid == 0)
      {
        chill_in_gallery(visitors_names[currentVisitorIdx - 1]);

        snprintf(sendBuffer, SEND_BUFFER_SIZE, "LEAVE %s", visitors_names[currentVisitorIdx - 1]);
        sentMessageLen = strlen(sendBuffer);

        printf("Visitor %s leaves\n", visitors_names[currentVisitorIdx - 1]);
        if (send(sock, sendBuffer, sentMessageLen, 0) != sentMessageLen)
        {
          DieWithError("send() sent a different number of bytes than expected");
        }

        if ((bytesRcvd = recv(sock, rcvBuffer, RCVBUFSIZE - 1, 0)) <= 0)
        {
          DieWithError("recv() failed or connection closed prematurely");
        }

        clearBuffer(rcvBuffer, RCVBUFSIZE);
        exit(0);
      }
    }
    else
    {
      printf("Visitor %s is trying to enter. But it is full\n", visitors_names[currentVisitorIdx]);
      sleep(1);
    }
  }

  for (int i = 0; i < numberOfVisitors; ++i)
  {
    wait(NULL);
  }

  clearBuffer(sendBuffer, SEND_BUFFER_SIZE);
  strcpy(sendBuffer, "CLOSE");
  sentMessageLen = strlen(sendBuffer);

  if (send(sock, sendBuffer, sentMessageLen, 0) != sentMessageLen)
  {
    DieWithError("send() sent a different number of bytes than expected");
  }

  for (int i = 0; i < ARTS_COUNT; ++i)
  {
    if (sem_close(arts[i].semaphore_ptr) == -1)
    {
      perror("Failed to close semaphore. Please try again\n");
      exit(1);
    }

    if (sem_unlink(arts[i].name) == -1)
    {
      perror("Failed to unlink semaphore. Please try again\n");
      exit(1);
    }

    free(arts_names[i]);
  }

  free(arts_names);

  for (int i = 0; i < numberOfVisitors; ++i)
  {
    free(visitors_names[i]);
  }

  free(visitors_names);

  printf("\nGallery is closed!\n");

  close(sock);
  exit(0);
}

#endif