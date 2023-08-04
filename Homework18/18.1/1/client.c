#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/my_socket"

int main() {
  int client_fd;
  struct sockaddr_un server_addr;
  char buffer[256];

  client_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (client_fd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(struct sockaddr_un));
  server_addr.sun_family = AF_LOCAL;
  strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

  if (connect(client_fd, (struct sockaddr *)&server_addr,
              sizeof(struct sockaddr_un)) == -1) {
    perror("connect");
    close(client_fd);
    exit(EXIT_FAILURE);
  }

  printf("Connecting to the server successfully!\n");

  const char *message = "Hello!";
  if (send(client_fd, message, strlen(message), 0) == -1) {
    perror("send");
    close(client_fd);
    exit(EXIT_FAILURE);
  }

  ssize_t num_bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
  if (num_bytes == -1) {
    perror("recv");
    close(client_fd);
    exit(EXIT_FAILURE);
  }

  // buffer[num_bytes] = '\0';
  printf("Received a response from the server: %s\n", buffer);

  close(client_fd);

  return 0;
}
