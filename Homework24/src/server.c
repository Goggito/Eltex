#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#define MULT_GROUP "225.0.1.1"
#define MULT_PORT 8888

int main() {
  struct sockaddr_in multicast_addr;
  int socket_fd;

  if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  int yes = 1;
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes,
                 sizeof(yes)) < 0) {
    perror("Reusing ADDR failed");
    return 1;
  }

  memset(&multicast_addr, 0, sizeof(multicast_addr));
  multicast_addr.sin_family = AF_INET;
  multicast_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  multicast_addr.sin_port = htons(MULT_PORT);

  if (bind(socket_fd, (struct sockaddr *)&multicast_addr,
           sizeof(multicast_addr)) < 0) {
    perror("bind");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }

  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr(MULT_GROUP);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq,
                 sizeof(mreq)) < 0) {
    perror("setsockopt");
    close(socket_fd);
    exit(EXIT_FAILURE);
  }

  char buffer[1024];
  while (1) {
    ssize_t recv_len =
        recvfrom(socket_fd, buffer, sizeof(buffer), 0, NULL, NULL);
    if (recv_len < 0) {
      perror("recvfrom");
      close(socket_fd);
      exit(EXIT_FAILURE);
    }
    buffer[recv_len] = '\0';
    printf("Received: %s", buffer);
  }

  close(socket_fd);
  return 0;
}
