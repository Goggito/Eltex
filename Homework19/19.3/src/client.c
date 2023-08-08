#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define SRV_IP "127.0.0.1"
#define SRV_PORT 8080

int get_free_port() {
  int sockfd;
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    perror("Ошибка при создании сокета");
    exit(EXIT_FAILURE);
  }

  memset(&addr, 0, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = 0;

  if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Ошибка при привязке сокета");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  if (getsockname(sockfd, (struct sockaddr *)&addr, &addr_len) < 0) {
    perror("Ошибка при получении порта");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  int free_port = ntohs(addr.sin_port);

  close(sockfd);

  return free_port;
}

int main() {
  int srv_sock, my_sock;
  struct sockaddr_in srv_addr, my_addr;
  char buffer[BUFF_SIZE];
  int my_port = get_free_port();

  // Server socket
  srv_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (srv_sock == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }
  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(SRV_PORT);
  inet_pton(AF_INET, SRV_IP, &(srv_addr.sin_addr));

  // Client socket
  my_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (my_sock == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }
  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(my_port);
  inet_pton(AF_INET, SRV_IP, &(my_addr.sin_addr));

  if (bind(my_sock, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
    perror("Error binding socket\n");
    close(my_sock);
    exit(EXIT_FAILURE);
  }

  char msg[] = "Hello Server!";
  strcpy(buffer, msg);

  if (sendto(srv_sock, &my_addr, sizeof(my_addr), 0,
             (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
    perror("Ошибка при отправке сообщения");
    close(srv_sock);
    exit(EXIT_FAILURE);
  }

  int received_bytes = recvfrom(my_sock, buffer, BUFF_SIZE, 0, NULL, NULL);
  if (received_bytes < 0) {
    perror("Ошибка при приеме ответа");
    close(my_sock);
    exit(EXIT_FAILURE);
  }

  buffer[received_bytes] = '\0';
  printf("Server response: %s\n", buffer);
  close(my_sock);

  close(srv_sock);
  return 0;
}
