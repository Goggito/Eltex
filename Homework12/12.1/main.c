#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_QUANTITY 5

void* threadFunc(void* thread_data){
	int* a = (int*)thread_data;
  printf("%d \n", *a);
	pthread_exit(0);
}

int main(int argc, char **argv) {
  
  int a[THREAD_QUANTITY];
  pthread_t pth[THREAD_QUANTITY];

  for (int i = 0; i < THREAD_QUANTITY; i++){
    a[i] = i;
    pthread_create(&pth[i], NULL, threadFunc, &a[i]);
  }

  for (int i = 0; i < THREAD_QUANTITY; i++){
    pthread_join(pth[i],NULL);
  }

  gets();
  return 0;
}