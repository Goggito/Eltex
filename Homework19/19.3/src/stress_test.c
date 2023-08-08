#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_USERS 10000
#define NUM_REQUESTS 1

#define BUFF_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
  int i, j;

  for (i = 0; i < NUM_USERS; i++) {
    pid_t pid = fork();

    if (pid == 0) {
      for (j = 0; j < NUM_REQUESTS; j++) {
        char* args[] = {"./client", NULL};
        // printf("CLIENT №%d \n", i);
        execv(args[0], args);
        perror("execv");
      }
      exit(0);
    } else if (pid < 0) {
      printf("Ошибка создания процесса\n");
      exit(1);
    }
  }

  for (i = 0; i < NUM_USERS; i++) {
    wait(NULL);
  }

  return 0;
}
