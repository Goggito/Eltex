#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

volatile sig_atomic_t flag = 0;

void handle_signal(int sig) { flag = 1; }

int main(int argc, char *argv[]) {
  printf("Server PID: %d\n", getpid());

  signal(SIGUSR1, handle_signal);

  while (1) {
    if (flag) {
      printf("Received SIGUSR1 signal!\n");
      flag = 0;
    }
  }

  return 0;
}