#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define SHM_SIZE 100

int main(int argc, char* argv[]) {
  int shm_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, 0666);
  if (shm_fd < 0) {
    perror("shm_open");
    exit(1);
  }

  if (ftruncate(shm_fd, sizeof(char) * SHM_SIZE) < 0) {
    perror("shm_open");
    exit(1);
  }

  void* shared_memory = mmap(NULL, sizeof(char) * SHM_SIZE,
                             PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shared_memory < 0) {
    perror("mmap");
    exit(1);
  }

  char* data = (char*)shared_memory;
  strcpy(data, "Hello!");
  while (1) {
    if (strcmp(data, "Hello!")) {
      printf("Data read from shared memory: %s\n", data);
      break;
    }
  }

  if (munmap(shared_memory, sizeof(char) * SHM_SIZE) < 0) {
    perror("munmap");
    exit(1);
  }

  if (shm_unlink("/shared_memory") < 0) {
    perror("shm_unlink");
    exit(1);
  }
  return 0;
}