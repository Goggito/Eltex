#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/udp_socket"

int main() {
  int client_fd, len;
  struct sockaddr_un server_addr;
  char buffer[1024];

  // Creating socket
  client_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);
  if (client_fd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_LOCAL;
  strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

  while (1) {
    printf("Enter a message to send (or 'exit' to quit): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // Remove trailing newline

    if (strcmp(buffer, "exit") == 0) {
      break;
    }

    // Send data to the server
    if (sendto(client_fd, buffer, strlen(buffer), 0,
               (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
      perror("sendto");
      close(client_fd);
      exit(EXIT_FAILURE);
    }

    // Receive response from the server (optional)
    len = sizeof(server_addr);
    int bytes_received = recvfrom(client_fd, buffer, sizeof(buffer) - 1, 0,
                                  (struct sockaddr*)&server_addr, &len);
    if (bytes_received == -1) {
      perror("recvfrom");
      close(client_fd);
      exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';
    printf("Received response from server: %s\n", buffer);
  }

  close(client_fd);
  return 0;
}
