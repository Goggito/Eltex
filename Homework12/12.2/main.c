#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define THREAD_QUANTITY 1000

void* threadFunc(void* thread_data) {
  int* a = (int*)thread_data;
  for (int i = 0; i < 100000; i++) {
    (*a)++;
  }
  pthread_exit(0);
}

int main(int argc, char** argv) {
  int a = 0;
  pthread_t pth[THREAD_QUANTITY];

  for (int i = 0; i < THREAD_QUANTITY; i++) {
    pthread_create(&pth[i], NULL, threadFunc, &a);
  }

  for (int i = 0; i < THREAD_QUANTITY; i++) {
    pthread_join(pth[i], NULL);
  }

  printf("a = %d \n", a);

  gets();
  return 0;
}