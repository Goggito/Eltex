#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/my_socket"

int main() {
  int server_fd, client_fd;
  struct sockaddr_un server_addr, client_addr;
  socklen_t client_len;
  char buffer[256];

  server_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(struct sockaddr_un));
  server_addr.sun_family = AF_LOCAL;
  strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

  if (bind(server_fd, (struct sockaddr *)&server_addr,
           sizeof(struct sockaddr_un)) == -1) {
    perror("bind");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 5) == -1) {
    perror("listen");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("The server is waiting for a connection...\n");

  client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
  if (client_fd == -1) {
    perror("accept");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("The client is connected!\n");

  ssize_t num_bytes;
  while ((num_bytes = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
    buffer[num_bytes] = '\0';
    printf("Message received from client: %s\n", buffer);

    const char *message = "Hi!";
    if (send(client_fd, message, strlen(message), 0) == -1) {
      perror("send");
      close(client_fd);
      close(server_fd);
      exit(EXIT_FAILURE);
    }
  }

  close(client_fd);
  close(server_fd);

  return 0;
}
