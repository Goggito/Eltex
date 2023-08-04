#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
  int client_socket;
  struct sockaddr_in server_address;
  char buffer[1024];

  client_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (client_socket == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr));

  while (1) {
    char msg[] = "Hello";
    strcpy(buffer, msg);

    if (sendto(client_socket, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_address,
               sizeof(server_address)) < 0) {
      perror("Error sending message");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    int received_bytes =
        recvfrom(client_socket, buffer, strlen(buffer), 0, NULL, NULL);
    if (received_bytes < 0) {
      perror("Error receiving response");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    buffer[received_bytes] = '\0';
    printf("Server response: %s\n", buffer);
    break;
  }

  close(client_socket);
  return 0;
}
