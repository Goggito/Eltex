#include "stdio.h"

#define N 10

void TaskOne() {
  int k = 1;

  int array[N][N];

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      array[i][j] = k++;
      printf(" %3d", array[i][j]);
    }
    printf("\n");
  }
}

void TaskTwo() {
  int array[N];

  for (int i = 0; i < N; i++) {
    array[i] = i + 1;
  }
  for (int i = 0; i < N; i++) {
    printf(" %d", array[i]);
  }
  printf("\n");

  for (int i = 0; i < N / 2; i++) {
    int swap = array[i];
    array[i] = array[N - 1 - i];
    array[N - 1 - i] = swap;
  }

  for (int i = 0; i < N; i++) {
    printf(" %d", array[i]);
  }
  printf("\n");
}

void TaskThree() {
  int array[N][N];
  int k = N - 1;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < k; j++) {
      array[i][j] = 0;
    }
    for (int j = k; j < N; j++) {
      array[i][j] = 1;
    }
    k--;
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf(" %d", array[i][j]);
    }
    printf("\n");
  }
}

void TaskFour() {
  int array[N][N];
  int number = 1;

  for (int x = 0; x < N / 2; x++) {
    for (int i = x; i < N - x - 1; i++) {
      array[x][i] = number++;
    }
    for (int i = x; i < N - x - 1; i++) {
      array[i][N - x - 1] = number++;
    }
    for (int i = N - x - 1; i > x; i--) {
      array[N - x - 1][i] = number++;
    }
    for (int i = N - x - 1; i > x; i--) {
      array[i][x] = number++;
    }

    if (N % 2 == 1) {
      array[N / 2][N / 2] = number++;
    }
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf(" %2d", array[i][j]);
    }
    printf("\n");
  }
}

int main() {
  printf("\n Task One: \n");
  TaskOne();

  printf("\n Task Two: \n");
  TaskTwo();

  printf("\n Task Three: \n");
  TaskThree();

  printf("\n Task Four: \n");
  TaskFour();
}