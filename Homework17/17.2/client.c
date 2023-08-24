#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int option = 0;
  int exit_check = 0;
  pid_t target_pid = 0;

  printf(" Enter server PID: ");
  scanf("%d", &target_pid);

  if (target_pid <= 0) {
    exit_check = 1;
  }

  system("clear");
  while (!exit_check) {
    system("clear");
    printf("\n     EventLoop  ");
    printf("\n 1 - Event");
    printf("\n 0 - Exit");
    printf("\n\n > ");
    scanf("%d", &option);

    switch (option) {
    case 0:
      system("clear");
      exit_check = 1;
      break;

    case 1:
      if (kill(target_pid, SIGUSR1) == 0) {
        printf("Signal sent successfully\n");
      } else {
        perror("kill");
      }
      system("clear");

      break;

    default:
      continue;
    }
  }

  return 0;
}
