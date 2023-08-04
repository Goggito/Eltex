#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/udp_socket"

int main() {
  int server_fd, len;
  struct sockaddr_un server_addr, client_addr;
  char buffer[1024];

  // Creating socket
  server_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
  if (server_fd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_LOCAL;
  strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

  // Bind the socket to a local address
  if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("bind");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on socket %s\n", SOCKET_PATH);

  while (1) {
    len = sizeof(client_addr);
    // Receive data from the client
    int bytes_received = recvfrom(server_fd, buffer, sizeof(buffer), 0,
                                  (struct sockaddr*)&client_addr, &len);
    if (bytes_received == -1) {
      perror("recvfrom");
      close(server_fd);
      exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';
    printf("Received message from client: %s\n", buffer);

    // You can process the received data here if needed

    // Sending a response back to the client (optional)
    if (sendto(server_fd, "Message received", 16, 0,
               (struct sockaddr*)&client_addr, len) == -1) {
      perror("sendto");
      close(server_fd);
      exit(EXIT_FAILURE);
    }
  }

  close(server_fd);
  return 0;
}
