#ifndef CLIENT_CHILL_IN_GALLERY

#define CLIENT_CHILL_IN_GALLERY

#include "./Common.h"

void swap(int *a, int *b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

void fill_random_order(int *array, int len)
{
  for (int i = 0; i < len; ++i)
  {
    array[i] = i;
  }

  for (int i = 0; i < len; ++i)
  {
    int a_idx = rand() % len;
    int b_idx = rand() % len;
    swap(array + a_idx, array + b_idx);
  }
}

void *chill_in_gallery(void *arg)
{
  char *visitor = (char *)arg;
  int arts_order[ARTS_COUNT];
  fill_random_order(arts_order, ARTS_COUNT);

  for (int i = 0; i < ARTS_COUNT; ++i)
  {
    struct Art current_art = arts[arts_order[i]];
    sem_wait(current_art.semaphore_ptr);
    printf("%s views %s \n", visitor, current_art.name);
    sleep(rand() % MAX_SLEEP_SECONDS);
    printf("%s stops viewing %s \n", visitor, current_art.name);
    sem_post(current_art.semaphore_ptr);
  }

  return NULL;
}

#endif