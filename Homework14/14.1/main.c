#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int fildes_to_parent[2];
  int fildes_to_child[2];

  pipe(fildes_to_parent);
  pipe(fildes_to_child);

  pid_t pid = fork();
  switch (pid) {
    case -1:
      perror("fork");
      return -1;

    // Child
    case 0:
      printf("child: w = %d r = %d\n", fildes_to_parent[1], fildes_to_child[0]);
      char messege[25] = "Hello from a child\n";
      if (write(fildes_to_parent[1], &messege, sizeof(messege)) < 0) {
        perror("Error writing to channel");
        return 1;
      }

      if (read(fildes_to_child[0], &messege, sizeof(messege)) < 0) {
        perror("Error reading from a channel");
        return 1;
      }
      printf("child: %s\n", messege);
      return 0;

    // Parent
    default: {
      char messege[25] = "Hello from a parent\n";
      printf("parent: w = %d r = %d\n", fildes_to_child[1],
             fildes_to_parent[0]);

      if (write(fildes_to_child[1], &messege, sizeof(messege)) < 0) {
        perror("Error writing to channel");
        return 1;
      }

      if (read(fildes_to_parent[0], &messege, sizeof(messege)) < 0) {
        perror("Error reading from a channel");
        return 1;
      }
      printf("parent: %s\n", messege);
      break;
    }
  }

  gets();
  return 0;
}