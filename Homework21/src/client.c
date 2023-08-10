#include <arpa/inet.h>
#include <netinet/ip.h>
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
  struct iphdr *ipheader = (struct iphdr *)buffer;
  struct udphdr *udpheader = (struct udphdr *)(buffer + sizeof(struct iphdr));

  raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if (raw_socket == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  int one = 1;
  const int *val = &one;
  if (setsockopt(raw_socket, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0) {
    perror("setsockopt");
    return 1;
  }

  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(8080);
  inet_pton(AF_INET, SERVER_IP, &(srv_addr.sin_addr));

  memset(buffer, 0, 4096);

  ipheader->ihl = 5;
  ipheader->version = 4;
  ipheader->tos = 0;
  ipheader->tot_len =
      sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(msg);
  ipheader->id = htonl(getpid());
  ipheader->frag_off = 0;
  ipheader->ttl = 255;
  ipheader->protocol = IPPROTO_UDP;
  ipheader->check = 0;
  ipheader->saddr = inet_addr("127.0.0.1");
  ipheader->daddr = srv_addr.sin_addr.s_addr;

  udpheader->source = htons(8888);
  udpheader->dest = htons(8080);
  udpheader->len = htons(sizeof(struct udphdr) + strlen(msg));
  udpheader->check = 0;

  memcpy(buffer + sizeof(struct iphdr) + sizeof(struct udphdr), msg,
         strlen(msg));

  if (sendto(raw_socket, buffer, ipheader->tot_len, 0,
             (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
    perror("Error accepting connection\n");
    close(raw_socket);
    exit(EXIT_FAILURE);
  }

  memset(buffer, 0, 4096);

  while (1) {
    int received_bytes = recvfrom(raw_socket, buffer, BUFF_SIZE, 0, NULL, NULL);
    if (received_bytes == -1) {
      perror("Error accepting connection\n");
      close(raw_socket);
      exit(EXIT_FAILURE);
    }

    struct iphdr *received_iph = (struct iphdr *)buffer;
    struct udphdr *received_udph =
        (struct udphdr *)(buffer + sizeof(struct iphdr));

    if (ntohs(received_udph->dest) == 8888) {
      printf("src-ip = %s \ndst-ip = %s",
             inet_ntoa(*(struct in_addr *)&received_iph->saddr),
             inet_ntoa(*(struct in_addr *)&received_iph->daddr));

      printf(
          "\nsrc-port = %d \ndst-port = %d \nlenght = %d \ncheck = %d "
          "\nmessage:%s\n\n",
          ntohs(received_udph->source), ntohs(received_udph->dest),
          ntohs(received_udph->len), ntohs(received_udph->check),
          (buffer + sizeof(struct udphdr) + sizeof(struct iphdr)));

      break;
    }
  }

  close(raw_socket);
  return 0;
}
