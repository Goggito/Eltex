#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHOP_QUANTITY 5
#define PROVIDER_QUANTITY 1
#define CONSUMER_QUANTITY 3

typedef struct {
  int number;
} Visitor;

pthread_mutex_t mutex;
pthread_cond_t condition;

int shop[SHOP_QUANTITY];
int occupied[SHOP_QUANTITY] = {0};

void* supply(void* thread_data) {
  Visitor* data = (Visitor*)thread_data;
  int number = data->number;

  while (1) {
    sleep(2);
    pthread_mutex_lock(&mutex);
    int available_index = -1;
    for (int i = 0; i < SHOP_QUANTITY; ++i) {
      if (occupied[i] == 0) {
        available_index = i;
        occupied[i] = 1;
        break;
      }
    }

    if (available_index == -1) {
      sleep(2);
      pthread_cond_wait(&condition, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    if (available_index != -1) {
      shop[available_index] += 500;
      printf(" Provider @%d: Shop[%d] = %d\n", number, available_index,
             shop[available_index]);

      pthread_mutex_lock(&mutex);
      occupied[available_index] = 0;
      pthread_cond_broadcast(&condition);
      pthread_mutex_unlock(&mutex);
    }
  }
}

void* purchase(void* thread_data) {
  Visitor* data = (Visitor*)thread_data;
  int number = data->number;

  int need = 10000;

  while (1) {
    pthread_mutex_lock(&mutex);
    int available_index = -1;
    for (int i = 0; i < SHOP_QUANTITY; ++i) {
      if (occupied[i] == 0) {
        available_index = i;
        occupied[i] = 1;
        break;
      }
    }

    if (available_index == -1) {
      pthread_cond_wait(&condition, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    if (available_index != -1) {
      if (need <= 0) {
        printf(" Consumer #%d: End\n", number);
        pthread_exit(NULL);
      }

      if (shop[available_index] > 0) {
        need -= shop[available_index];
        printf(" Consumer #%d: Shop[%d] = %d  Need = %d\n", number,
               available_index, shop[available_index], need);
        shop[available_index] = 0;
      }

      pthread_mutex_lock(&mutex);
      occupied[available_index] = 0;
      pthread_cond_broadcast(&condition);
      pthread_mutex_unlock(&mutex);
    }
  }

  pthread_exit(NULL);
}

int main(int argc, char** argv) {
  Visitor provider_data[PROVIDER_QUANTITY];
  Visitor consumer_data[CONSUMER_QUANTITY];
  pthread_t provider_pth[PROVIDER_QUANTITY];
  pthread_t consumer_pth[CONSUMER_QUANTITY];

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&condition, NULL);

  for (int i = 0; i < SHOP_QUANTITY; i++) {
    shop[i] = 1000;
  }

  for (int i = 0; i < PROVIDER_QUANTITY; i++) {
    printf(" Provider @%d: Create\n", i + 1);
    provider_data[i].number = i + 1;
    pthread_create(&provider_pth[i], NULL, supply, &provider_data[i]);
  }
  for (int i = 0; i < CONSUMER_QUANTITY; i++) {
    printf(" Consumer @%d: Create\n", i + 1);
    consumer_data[i].number = i + 1;
    pthread_create(&consumer_pth[i], NULL, purchase, &consumer_data[i]);
  }

  for (int i = 0; i < CONSUMER_QUANTITY; i++) {
    pthread_join(consumer_pth[i], NULL);
  }

  for (int i = 0; i < PROVIDER_QUANTITY; i++) {
    pthread_cancel(provider_pth[i]);
  }
  printf("END\n");

  pthread_mutex_destroy(&mutex);

  gets();
  return 0;
}