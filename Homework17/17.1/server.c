#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  pid_t target_pid = 1234;
  int signal_number = SIGKILL;

  printf("Enter PID: ");
  scanf("%d", &target_pid);

  if (kill(target_pid, signal_number) == 0) {
    printf("Signal %d successfully sent to process with PID %d\n",
           signal_number, target_pid);
  } else {
    perror("Error sending signal\n");
  }

  return 0;
}