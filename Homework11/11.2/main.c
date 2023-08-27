#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t pid;

  pid = fork();
  switch (pid) {
    case -1:
      perror("fork");
      return -1;
    case 0:
      printf("(2) My PID = %d PPID = %d \n", getpid(), getppid());

      pid = fork();
      switch (pid) {
        case -1:
          perror("fork");
          return -1;
        case 0:
          printf("(4) My PID = %d PPID = %d \n", getpid(), getppid());
          return 0;
        default: {
          int status;
          waitpid(pid, &status, 0);
          break;
        }
      }

      pid = fork();
      switch (pid) {
        case -1:
          perror("fork");
          return -1;
        case 0:
          printf("(5) My PID = %d PPID = %d \n", getpid(), getppid());
          return 0;
        default: {
          int status;
          waitpid(pid, &status, 0);
          break;
        }
      }

      return 0;
    default: {
      int status;
      waitpid(pid, &status, 0);
      printf("(1) My PID = %d PPID = %d \n", getpid(), getppid());
      break;
    }
  }

  pid = fork();
  switch (pid) {
    case -1:
      perror("fork");
      return -1;
    case 0:
      printf("(3) My PID = %d PPID = %d \n", getpid(), getppid());

      pid = fork();
      switch (pid) {
        case -1:
          perror("fork");
          return -1;
        case 0:
          printf("(6) My PID = %d PPID = %d \n", getpid(), getppid());
          return 0;
        default: {
          int status;
          waitpid(pid, &status, 0);
          break;
        }
      }

      return 0;
    default: {
      int status;
      waitpid(pid, &status, 0);
      break;
    }
  }

  gets();
  return 0;
}
