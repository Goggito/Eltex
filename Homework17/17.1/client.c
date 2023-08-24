#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  printf("PID: %d\n", getpid());
  while (1) {
    printf("I'm alive!!!\n");
    sleep(2);
  }

  printf("I you killed me\n");

  return 0;
}
