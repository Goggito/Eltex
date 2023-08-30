#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 4096

#define DEST_IP "198.168.31.249"
#define DEST_PORT 8080
#define DEST_MAC "08:00:27:86:28:0c"

#define SOURS_IP "192.168.31.227"
#define SOURS_PORT 8888
#define SOURCE_MAC "4c:cc:6a:d5:cd:96" // Ваш MAC адрес

unsigned short csum(unsigned short *buf, int n) {
  unsigned long sum;
  for (sum = 0; n > 0; n--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return (unsigned short)(~sum);
}

int main() {
  char buffer[BUFF_SIZE];
  char msg[] = "RAW Hello!";
  int raw_socket;
  struct sockaddr_ll srv_addr;

  memset(buffer, 0, BUFF_SIZE);

  // ADDRESS
  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sll_family = AF_PACKET;
  srv_addr.sll_protocol = htons(ETH_P_ALL);
  srv_addr.sll_ifindex = if_nametoindex("enp0s31f6");

  raw_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  if (raw_socket == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  struct ethhdr *eth = (struct ethhdr *)buffer;
  struct iphdr *ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
  struct udphdr *udp = (struct udphdr *)(buffer + (sizeof(struct iphdr) +
                                                   sizeof(struct ethhdr)));

  // ETHER
  sscanf(SOURCE_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth->h_source[0],
         &eth->h_source[1], &eth->h_source[2], &eth->h_source[3],
         &eth->h_source[4], &eth->h_source[5]);
  sscanf(DEST_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth->h_dest[0],
         &eth->h_dest[1], &eth->h_dest[2], &eth->h_dest[3], &eth->h_dest[4],
         &eth->h_dest[5]);
  eth->h_proto = htons(ETH_P_IP);

  // IP
  ip->ihl = 5;
  ip->version = 4;
  ip->tos = 0;
  ip->tot_len =
      htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(msg));
  ip->id = htonl(getpid());
  ip->frag_off = 0;
  ip->ttl = 20;
  ip->protocol = IPPROTO_UDP;
  ip->check = csum((unsigned short *)(buffer + sizeof(struct ethhdr)),
                   sizeof(struct iphdr) >> 1);
  ip->saddr = inet_addr(DEST_IP);
  ip->daddr = srv_addr.sll_addr;

  // UDP
  udp->source = htons(SOURS_PORT);
  udp->dest = htons(DEST_PORT);
  udp->len = htons(sizeof(struct udphdr) + strlen(msg));
  udp->check = csum(
      (unsigned short *)(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr)),
      sizeof(struct udphdr) + strlen(msg));
  ;

  memcpy(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr) +
             sizeof(struct udphdr),
         msg, strlen(msg));

  if (sendto(raw_socket, buffer,
             sizeof(struct iphdr) + sizeof(struct udphdr) +
                 sizeof(struct ethhdr) + strlen(msg),
             0, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
    perror("Error accepting connection\n");
    close(raw_socket);
    exit(EXIT_FAILURE);
  }

  memset(buffer, 0, BUFF_SIZE);

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

    if (ntohs(received_udph->dest) == SOURS_PORT) {
      printf("src-ip = %s \ndst-ip = %s",
             inet_ntoa(*(struct in_addr *)&received_iph->saddr),
             inet_ntoa(*(struct in_addr *)&received_iph->daddr));

      printf("\nsrc-DEST_PORT = %d \ndst-DEST_PORT = %d \nlength = %d \ncheck "
             "= %d "
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
