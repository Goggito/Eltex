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
#define SERVER_NUMBER 10
#define PORT 8080

typedef struct {
  int numb;
  int port;
  int cond;
} SrvInfo;

SrvInfo perm_srv[SERVER_NUMBER];
pthread_mutex_t mutex;

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

int time_get(char *buffer) {
  time_t current_time;
  current_time = time(NULL);

  if (current_time == (time_t)-1) {
    perror("Error getting time");
    return 1;
  }
  char *time_msg = ctime(&current_time);

  strcpy(buffer, time_msg);
  return 0;
}

void *new_srv(void *thread_data) {
  int numb = (int)thread_data;
  int srv_sock;
  struct sockaddr_in srv_addr, cli_addr;
  socklen_t cli_len = sizeof(cli_addr);
  char buffer[BUFF_SIZE];

  printf("%d:new_srv - port: %d\n", numb, perm_srv[numb].port);
  srv_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (srv_sock == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(perm_srv[numb].port);
  inet_pton(AF_INET, SERVER_IP, &(srv_addr.sin_addr));

  if (bind(srv_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1) {
    perror("Error binding socket\n");
    close(srv_sock);
    exit(EXIT_FAILURE);
  }

  while (1) {
    int received_bytes = recvfrom(srv_sock, buffer, BUFF_SIZE, 0,
                                  (struct sockaddr *)&cli_addr, &cli_len);
    if (received_bytes == -1) {
      perror("Error accepting connection\n");
      close(srv_sock);
      exit(EXIT_FAILURE);
    }

    printf("%d:new_srv \n", numb);

    time_get(buffer);

    if (sendto(srv_sock, buffer, BUFF_SIZE, 0, (struct sockaddr *)&cli_addr,
               sizeof(cli_addr)) < 0) {
      perror("Ошибка при отправке ответа\n");
      close(srv_sock);
      exit(EXIT_FAILURE);
    }

    perm_srv[numb].cond = 1;
  }

  close(srv_sock);
  return 0;
}

void *new_over_srv(void *thread_data) {
  int *port = (int *)thread_data;
  int srv_sock;
  struct sockaddr_in srv_addr, cli_addr;
  socklen_t cli_len = sizeof(cli_addr);
  char buffer[BUFF_SIZE];

  printf("%d:new_over_srv - port: %d\n", *port, *port);
  srv_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (srv_sock == -1) {
    perror("Error creating socket\n");
    exit(EXIT_FAILURE);
  }

  memset(&srv_addr, 0, sizeof(srv_addr));
  srv_addr.sin_family = AF_INET;
  srv_addr.sin_port = htons(*port);
  inet_pton(AF_INET, SERVER_IP, &(srv_addr.sin_addr));

  if (bind(srv_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) == -1) {
    perror("Error binding socket\n");
    close(srv_sock);
    exit(EXIT_FAILURE);
  }

  pthread_mutex_unlock(&mutex);

  int received_bytes = recvfrom(srv_sock, buffer, BUFF_SIZE, 0,
                                (struct sockaddr *)&cli_addr, &cli_len);
  if (received_bytes == -1) {
    perror("Error accepting connection\n");
    close(srv_sock);
    exit(EXIT_FAILURE);
  }

  time_get(buffer);

  if (sendto(srv_sock, buffer, BUFF_SIZE, 0, (struct sockaddr *)&cli_addr,
             sizeof(cli_addr)) < 0) {
    perror("Ошибка при отправке ответа\n");
    close(srv_sock);
    exit(EXIT_FAILURE);
  }

  close(srv_sock);
  return 0;
}

int get_free_srv() {
  for (int i = 0; i < SERVER_NUMBER; i++) {
    if (perm_srv[i].cond == 1) {
      perm_srv[i].cond = 0;
      return perm_srv[i].port;
    }
  }

  return -1;
}

int main() {
  int srv_sock;
  struct sockaddr_in srv_addr, cli_addr;
  socklen_t cli_len = sizeof(cli_addr);

  pthread_t over_srv_tid;
  pthread_t srv_tid[SERVER_NUMBER];
  pthread_mutex_init(&mutex, NULL);

  int client_count = 0;
  char buffer[BUFF_SIZE];

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

  for (int i = 0; i < SERVER_NUMBER; i++) {
    int port = get_free_port();
    perm_srv[i].cond = 1;
    perm_srv[i].port = port;

    if (pthread_create(&srv_tid[i], NULL, new_srv, i) != 0) {
      perror("pthread_create");
      return 1;
    }
  }

  printf("The server is waiting for a connection...\n");
  while (1) {
    int received_bytes = recvfrom(srv_sock, buffer, BUFF_SIZE, 0,
                                  (struct sockaddr *)&cli_addr, &cli_len);
    if (received_bytes == -1) {
      perror("Error accepting connection\n");
      close(srv_sock);
      exit(EXIT_FAILURE);
    }

    buffer[received_bytes] = '\0';
    client_count++;
    // printf("Получено от клиента %d: %s\n", client_count, buffer);

    int port = get_free_srv();
    if (port < 0) {
      port = get_free_port();

      pthread_mutex_lock(&mutex);

      if (pthread_create(&over_srv_tid, NULL, new_over_srv, &port) != 0) {
        perror("pthread_create");
        return 1;
      }

      pthread_mutex_lock(&mutex);
    }

    if (sendto(srv_sock, &port, sizeof(int), 0, (struct sockaddr *)&cli_addr,
               sizeof(cli_addr)) < 0) {
      perror("Ошибка при отправке ответа");
      close(srv_sock);
      exit(EXIT_FAILURE);
    }

    pthread_mutex_unlock(&mutex);
  }

  for (int i = 0; i < SERVER_NUMBER; i++) {
    pthread_cancel(srv_tid[i]);
  }

  close(srv_sock);
  return 0;
}
