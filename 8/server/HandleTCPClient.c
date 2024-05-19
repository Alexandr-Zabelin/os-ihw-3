#include "./Common.h"

#define COMMANDS_SEP ' '
#define MAX_VISITORS 25
#define ANS_BUFFER_SIZE 2
#define RCV_BUFFER_SIZE 64

void clearBuffer(char *str)
{
    memset(str, 0, RCV_BUFFER_SIZE);
}

int getStartParamsIdx(char *message, int messageLen)
{
    for (int i = 0; i < messageLen; ++i)
    {
        if (message[i] == COMMANDS_SEP)
        {
            return i + 1;
        }
    }
    return messageLen;
}

void HandleTCPClient(int clntSocket)
{
    int numberOfVisitorsInGallery = 0;
    char galleryName[RCV_BUFFER_SIZE];
    char ansBuffer[ANS_BUFFER_SIZE] = "Y";
    char recvBuffer[RCV_BUFFER_SIZE];
    int recvMsgSize;

    if ((recvMsgSize = recv(clntSocket, recvBuffer, RCV_BUFFER_SIZE, 0)) < 0)
    {
        DieWithError("recv() failed");
    }

    int paramsIdx = getStartParamsIdx(recvBuffer, recvMsgSize);

    strncpy(galleryName, recvBuffer + paramsIdx, recvMsgSize - paramsIdx);
    galleryName[recvMsgSize - paramsIdx] = '\0';
    printf("Start serving %s gallery\n", galleryName);
    clearBuffer(recvBuffer);

    while (1)
    {
        if (send(clntSocket, ansBuffer, ANS_BUFFER_SIZE, 0) != ANS_BUFFER_SIZE)
        {
            DieWithError("send() failed");
        }

        if ((recvMsgSize = recv(clntSocket, recvBuffer, RCV_BUFFER_SIZE, 0)) < 0)
        {
            DieWithError("recv() failed");
        }

        int sepIdx = getStartParamsIdx(recvBuffer, recvMsgSize);

        if (recvBuffer[0] == 'E')
        {
            ansBuffer[0] = numberOfVisitorsInGallery < MAX_VISITORS ? 'Y' : 'N';
            if (ansBuffer[0] == 'Y')
            {
                ++numberOfVisitorsInGallery;
            }
        }

        if (recvBuffer[0] == 'L')
        {
            ansBuffer[0] = 'Y';
            --numberOfVisitorsInGallery;
        }

        if (recvBuffer[0] == 'C')
        {
            break;
        }

        clearBuffer(recvBuffer);
    }

    printf("Finish serving %s gallery\n", galleryName);
    close(clntSocket);
}
