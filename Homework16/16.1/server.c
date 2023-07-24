#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main(int argc, char* argv[]) {
  int shmid;
  key_t key;

  key = ftok("/home/user/Eltex/Homework16/16.1/Makefile", 1);
  if (key < 0) {
    perror("ftok");
    exit(1);
  }

  shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
  if (shmid < 0) {
    perror("shmget");
    exit(1);
  }

  char* shared_memory = (char*)shmat(shmid, (void*)0, 0);
  if (shared_memory == (char*)-1) {
    perror("shmat");
    exit(1);
  }

  sprintf(shared_memory, "Hello!");
  while (1) {
    if (strcmp(shared_memory, "Hello!")) {
      printf("Data read from shared memory: %s\n", shared_memory);
      break;
    }
  }

  if (shmdt(shared_memory) == -1) {
    perror("shmdt");
    exit(1);
  }

  if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    perror("shmctl");
    exit(1);
  }

  return 0;
}