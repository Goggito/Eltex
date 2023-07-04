#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

int main() {

  int option = 0;
  int exit_cheak = 0;
  int a = 0;
  int b = 0;
  int result = 0;

  system("clear");
  while (!exit_cheak) {
    system("clear");
    printf("\n     Calculator  ");
    printf("\n 1 - +");
    printf("\n 2 - -");
    printf("\n 3 - *");
    printf("\n 4 - /");
    printf("\n 0 - Exit");
    printf("\n\n Result > %d", result);
    printf("\n\n > ");
    scanf("%d", &option);

    switch (option) {
      case 0:
        system("clear");
        exit_cheak = 1;
        break;

      case 1:
        system("clear");
        printf("\n Enter A number:");
        scanf(" %d", &a);
        printf("\n Enter B number:");
        scanf(" %d", &b);
        result = my_add (a, b);
        break;

      case 2:
        system("clear");
        printf("\n Enter A number:");
        scanf(" %d", &a);
        printf("\n Enter B number:");
        scanf(" %d", &b);
        result = my_sub(a, b);
        break;

      case 3:
        system("clear");
        printf("\n Enter A number:");
        scanf(" %d", &a);
        printf("\n Enter B number:");
        scanf(" %d", &b);
        result = my_mul(a, b);
        break;

      case 4:
        system("clear");
        printf("\n Enter A number:");
        scanf(" %d", &a);
        printf("\n Enter B number:");
        scanf(" %d", &b);
        result = my_div(a, b);
        break;

      default:
        break;
    }
    
  }

  return 0;
}