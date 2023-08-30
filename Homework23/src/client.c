#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 4096

#define DEST_IP "192.168.31.255"
#define DEST_PORT 8080
#define DEST_MAC "ff:ff:ff:ff:ff:ff"

#define SOURS_IP "127.0.0.1"
#define SOURS_PORT 8888
#define SOURCE_MAC "00:11:22:33:44:55" // Ваш MAC адрес

int main() {
  char buffer[BUFF_SIZE];
  char msg[] = "Broadcast Hello!";

  int socket_fd;
  struct sockaddr_in broadcast_addr;


  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  int one = 1;
  const int *val = &one;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, val, sizeof(one)) < 0) {
    perror("setsockopt");
    return 1;
  }

  memset(&broadcast_addr, 0, sizeof(broadcast_addr));
  broadcast_addr.sin_family = AF_INET;
  broadcast_addr.sin_port = htons(DEST_PORT);
  broadcast_addr.sin_addr.s_addr = inet_addr(DEST_IP);

  if (sendto(socket_fd, msg, strlen(msg), 0, (struct sockaddr *)&broadcast_addr,
             sizeof(broadcast_addr)) < 0) {
    perror("Error accepting connection\n");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }

  while (1) {
    int received_bytes = recvfrom(socket_fd, buffer, BUFF_SIZE, 0, NULL, NULL);
    if (received_bytes == -1) {
      perror("Error accepting connection\n");
      close(socket_fd);
      exit(EXIT_FAILURE);
    }

    break;
  }

  close(socket_fd);
  return 0;
}
