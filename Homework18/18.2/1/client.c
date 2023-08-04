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

  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr));

  if (connect(client_socket, (struct sockaddr *)&server_address,
              sizeof(server_address)) == -1) {
    perror("Connection error\n");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  printf("Connecting to the server successfully!\n");

  while (1) {
    char msg[] = "Hello";
    strcpy(buffer, msg);

    send(client_socket, buffer, strlen(buffer), 0);

    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
      break;
    }
    buffer[bytes_received] = '\0';
    printf("Server response: %s\n", buffer);
    break;
  }

  close(client_socket);
  return 0;
}
