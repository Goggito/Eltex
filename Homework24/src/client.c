#include <linux/ip.h>
#include <linux/udp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MULT_GROUP "225.0.1.1"
#define MULT_PORT 8888

int main() {
  char msg[] = "Multicast Hello!";

  int socket_fd;
  struct sockaddr_in multicast_addr;

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  memset(&multicast_addr, 0, sizeof(multicast_addr));
  multicast_addr.sin_family = AF_INET;
  multicast_addr.sin_port = htons(MULT_PORT);
  multicast_addr.sin_addr.s_addr = inet_addr(MULT_GROUP);

  if (sendto(socket_fd, msg, strlen(msg), 0, (struct sockaddr *)&multicast_addr,
             sizeof(multicast_addr)) < 0) {
    perror("Error accepting connection\n");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }

  close(socket_fd);
  return 0;
}
