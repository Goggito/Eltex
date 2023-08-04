#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main() {
  int server_socket, client_socket;
  struct sockaddr_in server_address, client_address;
  socklen_t client_len = sizeof(client_address);
  char buffer[1024];

  server_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_socket == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) == -1) {
    perror("Error binding socket\n");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("The server is waiting for a connection...\n");

  while (1) {
    int received_bytes =
        recvfrom(server_socket, buffer, strlen(buffer), 0,
                 (struct sockaddr *)&client_address, &client_len);
    if (received_bytes == -1) {
      perror("Error accepting connection\n");
      close(server_socket);
      exit(EXIT_FAILURE);
    }

    buffer[received_bytes] = '\0';
    printf("Received from client: %s\n", buffer);

    char response[] = "Hi";
    if (sendto(server_socket, response, strlen(response), 0,
               (struct sockaddr *)&client_address,
               sizeof(client_address)) < 0) {
      perror("Error sending response");
      close(server_socket);
      exit(EXIT_FAILURE);
    }
  }

  close(server_socket);
  return 0;
}
