#ifndef COMMON_DIE_WITH_ERROR

#define COMMON_DIE_WITH_ERROR

#include <stdio.h>  /* for perror() */
#include <stdlib.h> /* for exit() */

void DieWithError(char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}

#endif
