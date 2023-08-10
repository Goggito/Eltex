#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define PORT 8080

int main() {
  int packet_counter = 0;
  int raw_socket;
  struct sockaddr_in raw_addr;

  raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (raw_socket == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    int received_bytes = recvfrom(raw_socket, NULL, NULL, 0, NULL, NULL);
    if (received_bytes == -1) {
      perror("Error accepting connection\n");
      close(raw_socket);
      exit(EXIT_FAILURE);
    }

    packet_counter++;
    system("clear");
    printf("Packet count: %d\n", packet_counter);
  }

  close(raw_socket);

  return 0;
}