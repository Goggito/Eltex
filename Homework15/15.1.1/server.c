#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

struct msgbuf {
  long mtype;
  char mtext[300];
};

static void send_msg(int qid, int msgtype, char* msg_buff) {
  struct msgbuf msg;

  msg.mtype = msgtype;

  snprintf(msg.mtext, sizeof(msg.mtext), "a message at %s ", msg_buff);

  if (msgsnd(qid, &msg, sizeof(msg.mtext), 0) == -1) {
    perror("msgsnd error");
    exit(EXIT_FAILURE);
  }
  printf("sent: %s\n", msg.mtext);
}

static void get_msg(int qid, int msgtype) {
  struct msgbuf msg;

  if (msgrcv(qid, &msg, sizeof(msg.mtext), msgtype, 0) == -1) {
    if (errno != ENOMSG) {
      perror("msgrcv");
      exit(EXIT_FAILURE);
    }
    printf("No message available for msgrcv()\n");
  } else
    printf("message received: %s\n", msg.mtext);
}

int main(int argc, char* argv[]) {
  int qid;
  char msg[20] = "Hello";

  key_t msgkey = ftok("Homework15/15.1.1/Makefile", 1);
  qid = msgget(msgkey, IPC_CREAT | 0666);

  if (qid == -1) {
    perror("msgget");
    exit(EXIT_FAILURE);
  }

  send_msg(qid, 1, msg);
  get_msg(qid, 123);

  exit(EXIT_SUCCESS);
}