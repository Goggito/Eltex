#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
  int srv_sock;
  struct sockaddr_in srv_addr, cli_addr;
  socklen_t cli_len = sizeof(cli_addr);

  char buffer[BUFF_SIZE];
  char buffer_send[BUFF_SIZE];

  srv_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (srv_sock == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, SERVER_IP, &(srv_addr.sin_addr));

  if (bind(srv_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1) {
    perror("Error binding socket\n");
    close(srv_sock);
    exit(EXIT_FAILURE);
  }

  printf("The server is waiting for a connection...\n");

  while (1) {
    int received_bytes = recvfrom(srv_sock, buffer, BUFF_SIZE + 20, 0,
                                  (struct sockaddr *)&cli_addr, &cli_len);
    if (received_bytes == -1) {
      perror("Error accepting connection\n");
      close(srv_sock);
      exit(EXIT_FAILURE);
    }

    buffer[received_bytes] = '\0';
    strcpy(buffer_send, buffer);

    printf("Получено от клиента: %s\n", buffer);

    strcat(buffer_send, " + server \0");

    if (sendto(srv_sock, buffer_send, strlen(buffer_send), 0,
               (struct sockaddr *)&cli_addr, sizeof(cli_addr)) < 0) {
      perror("Ошибка при отправке ответа");
      close(srv_sock);
      exit(EXIT_FAILURE);
    }

    printf("Отправлено клиенту: %s\n", buffer_send);
  }

  close(srv_sock);
  return 0;
}
