#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
  const char* srv_queue_name = "/server_queue";
  const char* cli_queue_name = "/client_queue";
  mode_t permissions = S_IRUSR | S_IWUSR;
  mqd_t srv_mqd, cli_mqd;

  struct mq_attr attr;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = 150;

  char msg_send[100] = "Hello";
  char msg_recive[150];

  srv_mqd = mq_open(srv_queue_name, O_CREAT | O_RDWR, permissions, &attr, NULL);
  if (srv_mqd == (mqd_t)-1) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }
  cli_mqd = mq_open(cli_queue_name, O_CREAT | O_RDWR, permissions, &attr, NULL);
  if (cli_mqd == (mqd_t)-1) {
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  if (mq_send(srv_mqd, msg_send, strlen(msg_send) + 1, 0) == -1) {
    perror("mq_send");
    exit(EXIT_FAILURE);
  }

  if (mq_receive(cli_mqd, msg_recive, sizeof(msg_recive), 0) == -1) {
    perror("mq_receive");
    exit(EXIT_FAILURE);
  }

  printf("%s\n", msg_recive);

  if (mq_close(srv_mqd) == -1) {
    perror("mq_close");
    exit(EXIT_FAILURE);
  }

  if (mq_close(cli_mqd) == -1) {
    perror("mq_close");
    exit(EXIT_FAILURE);
  }

  mq_unlink(srv_queue_name);
  mq_unlink(cli_queue_name);

  exit(EXIT_SUCCESS);
}