#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BUFF_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_NUMBER 10
#define PORT 8080

struct msgbuf {
  long mtype;
  struct sockaddr_in addr;
};

int qid;
pthread_mutex_t mutex;

static void send_msg(int qid, int msgtype, struct sockaddr_in addr) {
  struct msgbuf msg;

  msg.mtype = msgtype;
  msg.addr = addr;

  if (msgsnd(qid, &msg, sizeof(msg.addr), IPC_NOWAIT) == -1) {
    perror("msgsnd error");
    exit(EXIT_FAILURE);
  }
}

static void get_msg(int qid, int msgtype, struct sockaddr_in *addr) {
  struct msgbuf msg;

  if (msgrcv(qid, &msg, sizeof(msg.addr), msgtype, MSG_NOERROR) == -1) {
    if (errno != ENOMSG) {
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }
    printf("No message available for msgrcv()\n");
  }

  *addr = msg.addr;
}

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
  int *numb = (int *)thread_data;
  int srv_sock;
  char buffer[BUFF_SIZE];

  printf("%d:new_srv - created \n", *numb);

  while (1) {
    struct sockaddr_in cli_addr;

    pthread_mutex_lock(&mutex);
    printf("%d:new_srv - wait \n", *numb);
    get_msg(qid, 1, &cli_addr);
    pthread_mutex_unlock(&mutex);

    char ip_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &cli_addr.sin_addr, ip_addr, INET_ADDRSTRLEN);
    printf("%d:new_srv - cli_info %s:%d  \n", *numb, ip_addr,
           htons(cli_addr.sin_port));

    time_get(buffer);

    int cli_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (cli_sock == -1) {
      perror("Error creating socket\n");
      exit(EXIT_FAILURE);
    }

    if (sendto(cli_sock, buffer, BUFF_SIZE, 0, (struct sockaddr *)&cli_addr,
               sizeof(cli_addr)) < 0) {
      perror("Ошибка при отправке ответа\n");
      close(cli_sock);
      exit(EXIT_FAILURE);
    }

    close(cli_sock);
  }

  close(srv_sock);
  return 0;
}

int main() {
  int srv_sock;
  struct sockaddr_in srv_addr, cli_addr;

  pthread_t srv_tid[SERVER_NUMBER];
  int thread_arg[SERVER_NUMBER];
  pthread_mutex_init(&mutex, NULL);

  key_t msgkey;

  int client_count = 0;

  // Main server socket
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

  // Queue
  msgkey = ftok("../obj/server.o", 1);
  qid = msgget(msgkey, IPC_CREAT | 0666);

  // New server init
  for (int i = 0; i < SERVER_NUMBER; i++) {
    thread_arg[i] = i;
    if (pthread_create(&srv_tid[i], NULL, new_srv, &thread_arg[i]) != 0) {
      perror("pthread_create");
      return 1;
    }
  }

  // Main loop
  printf("The server is waiting for a connection...\n");
  while (1) {
    int received_bytes =
        recvfrom(srv_sock, &cli_addr, sizeof(cli_addr), 0, NULL, NULL);
    if (received_bytes == -1) {
      perror("SERVER: Error accepting connection\n");
      close(srv_sock);
      exit(EXIT_FAILURE);
    }

    client_count++;

    send_msg(qid, 1, cli_addr);
  }

  for (int i = 0; i < SERVER_NUMBER; i++) {
    pthread_cancel(srv_tid[i]);
  }

  close(srv_sock);
  return 0;
}
