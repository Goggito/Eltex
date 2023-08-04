#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
  int server_socket, client_socket;
  struct sockaddr_in server_address, client_address;
  char buffer[1024];

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr *)&server_address,
           sizeof(server_address)) == -1) {
    perror("Error binding socket\n");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  if (listen(server_socket, 5) == -1) {
    perror("Error while waiting for connections\n");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("The server is waiting for a connection...\n");

  while (1) {
    socklen_t client_address_length = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address,
                           &client_address_length);
    if (client_socket == -1) {
      perror("Error accepting connection\n");
      close(server_socket);
      exit(EXIT_FAILURE);
    }

    printf("Connected client with IP: %s, port: %d\n",
           inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    while (1) {
      int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
      if (bytes_received <= 0) {
        break;
      }
      buffer[bytes_received] = '\0';
      printf("Received from client: %s\n", buffer);

      char response[] = "Hi";
      send(client_socket, response, strlen(response), 0);
    }

    printf("Client with IP: %s, port: %d disconnected\n",
           inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    close(client_socket);
  }

  close(server_socket);
  return 0;

  return 0;
}
