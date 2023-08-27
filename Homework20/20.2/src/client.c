#include <arpa/inet.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 4096
#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
  char buffer[BUFF_SIZE];
  char msg[] = "RAW Hello!";

  int raw_socket;
  struct sockaddr_in srv_addr;
  struct udphdr *udpheader = (struct udphdr *)buffer;

  raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (raw_socket == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  memset(buffer, 0, 4096);

  udpheader->source = htons(8888);
  udpheader->dest = htons(8080);
  udpheader->len = htons(sizeof(struct udphdr) + strlen(msg));
  udpheader->check = 0;

  memcpy(buffer + sizeof(struct udphdr), msg, strlen(msg));

  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(8080);
  inet_pton(AF_INET, SERVER_IP, &(srv_addr.sin_addr));

  if (sendto(raw_socket, buffer, strlen(buffer), 0,
             (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
    perror("Error accepting connection\n");
    close(raw_socket);
    exit(EXIT_FAILURE);
  }

  while (1) {
    int received_bytes = recvfrom(raw_socket, buffer, BUFF_SIZE, 0, NULL, NULL);
    if (received_bytes == -1) {
      perror("Error accepting connection\n");
      close(raw_socket);
      exit(EXIT_FAILURE);
    }

    printf("\n\n%s\n", buffer);
  }

  close(raw_socket);
  return 0;
}
