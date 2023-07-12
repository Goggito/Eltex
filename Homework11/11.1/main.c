#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t pid = fork();
  switch (pid) {
    case -1:
      perror("fork");
      return -1;
    case 0:
      printf("Hello from Child! My PID = %d PPID = %d \n", getpid(), getppid());
      return 0;
    default: {
      int status;
      waitpid(pid, &status, 0);
      printf("Hello from Parent! My PID = %d PPID = %d \n", getpid(),
             getppid());
      break;
    }
  }

  return 0;
}