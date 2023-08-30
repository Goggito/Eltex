#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEST_IP "192.168.31.255"
#define DEST_PORT 8080
#define DEST_MAC "ff:ff:ff:ff:ff:ff"

int main() {
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

  close(socket_fd);
  return 0;
}
