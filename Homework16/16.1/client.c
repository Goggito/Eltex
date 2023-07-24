#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int shmid;
  key_t key;
  char *shmaddr;

  key = ftok("/home/user/Eltex/Homework16/16.1/Makefile", 1);
  if (key < 0) {
    perror("ftok");
    exit(1);
  }

  shmid = shmget(key, 0, 0);
  if (shmid < 0) {
    perror("shmget");
    exit(1);
  }

  shmaddr = (char *)shmat(shmid, NULL, 0);
  if (shmaddr == (char *)-1) {
    perror("shmat");
    exit(1);
  }

  char *shared_memory = (char *)shmat(shmid, (void *)0, 0);
  if (shared_memory == (char *)-1) {
    perror("shmat");
    exit(1);
  }

  shared_memory = (char *)shmat(shmid, (void *)0, 0);
  if (shared_memory == (char *)-1) {
    perror("shmat");
    exit(1);
  }

  printf("Data read from shared memory: %s\n", shared_memory);
  sprintf(shared_memory, "Hi!");

  if (shmdt(shmaddr) == -1) {
    perror("shmdt");
    exit(1);
  }

  return 0;
}
