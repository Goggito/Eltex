#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 100
#define MAX_NAME_LENGTH 20
#define MAX_SURNAME_LENGTH 20
#define MAX_NUMBER_LENGTH 15

typedef struct {
  char name[MAX_NAME_LENGTH];
  char surname[MAX_SURNAME_LENGTH];
  char number[MAX_NUMBER_LENGTH];
} Subscriber;

void PhonebookMove(Subscriber *book, int A_num, int B_num) {
  strncpy(book[B_num].surname, book[A_num].surname, MAX_SURNAME_LENGTH);
  strncpy(book[B_num].name, book[A_num].name, MAX_NAME_LENGTH);
  strncpy(book[B_num].number, book[A_num].number, MAX_NUMBER_LENGTH);
}

void PhonebookPrint(Subscriber *book, int entry_number) {
  int i;

  if (entry_number == 0) {
    printf("Phonebook is empty.\n");
    return;
  }

  printf("№     Фамилия             Имя                 Номер\n");

  for (i = 0; i < entry_number; i++) {
    printf("%-5d %-20s %-20s %-20s\n", i + 1, book[i].surname, book[i].name,
           book[i].number);
  }
}

int PhonebookAdd(Subscriber *book, int *entry_number) {
  if (*entry_number >= MAX_ENTRIES) {
    return -1;
  }

  char name[MAX_NAME_LENGTH];
  char surname[MAX_SURNAME_LENGTH];
  char number[MAX_NUMBER_LENGTH];
  int i = 0;

  system("clear");
  printf(" Enter last name:");
  scanf(" %s", surname);
  while (surname[i] != '\0') {
    if (!isalpha(surname[i])) {
      return -1;
    }
    i++;
  }
  system("clear");

  i = 0;
  printf(" Enter name:");
  scanf(" %s", name);
  while (name[i] != '\0') {
    if (!isalpha(name[i])) {
      return -1;
    }
    i++;
  }
  system("clear");

  i = 0;
  printf(" Enter number:");
  scanf(" %s", number);
  while (number[i] != '\0') {
    if (!isdigit(number[i])) {
      return -1;
    }
    i++;
  }
  system("clear");

  strcpy(book[*entry_number].surname, surname);
  strcpy(book[*entry_number].name, name);
  strcpy(book[*entry_number].number, number);

  *entry_number += 1;

  return 0;
}

int PhonebookDelete(Subscriber *book, int *entry_number, int delete_number) {
  if (delete_number > *entry_number) {
    return -1;
  }

  int swap_number = 0;
  int i = 0;

  while (book[delete_number].surname[i] != '\0') {
    book[delete_number].surname[i] = '\0';
    i++;
  }

  i = 0;
  while (book[delete_number].name[i] != '\0') {
    book[delete_number].name[i] = '\0';
    i++;
  }

  i = 0;
  while (book[delete_number].number[i] != '\0') {
    book[delete_number].number[i] = '\0';
    i++;
  }

  swap_number = delete_number + 1;
  while (*entry_number != swap_number) {
    PhonebookMove(book, swap_number, swap_number - 1);
    swap_number += 1;
  }

  *entry_number -= 1;
  system("clear");
  return 0;
}

int PhonebookSearch(Subscriber *book) {
  char surname[MAX_SURNAME_LENGTH];
  int i = 0;

  printf("Enter a surname to search: ");
  scanf("%s", surname);

  for (i = 0; i < MAX_ENTRIES; i++) {
    if (strcmp(book[i].name, surname) == 0) {
      printf("Result:\n");
      printf("%-5d %-20s %-20s %-20s\n", i + 1, book[i].surname, book[i].name,
             book[i].number);
      return 0;
    }
  }

  return -1;
}

int main() {
  Subscriber phonebook[MAX_ENTRIES];
  int entry_number = 0;
  int delete_number = 0;
  int option = 0;
  int exit_cheak = 0;
  int exit_cheak_case = 0;

  system("clear");
  while (!exit_cheak) {
    printf("\n     Phonebook  ");
    printf("\n 1 - Add subscriber");
    printf("\n 2 - Delete subscriber");
    printf("\n 3 - Search subscriber");
    printf("\n 4 - Print phonebook");
    printf("\n 0 - Exit");

    printf("\n\n > ");
    scanf("%d", &option);

    switch (option) {
      case 0:
        exit_cheak = 1;
        break;

      case 1:
        system("clear");
        if (PhonebookAdd(phonebook, &entry_number)) {
          system("clear");
          printf(" Error: Incorrect data entered!\n");
        }

        break;

      case 2:
        system("clear");
        printf(" Enter the number of the entry you want to delete: ");
        scanf("%d", &delete_number);

        if (PhonebookDelete(phonebook, &entry_number, delete_number)) {
          system("clear");
          printf(" Error: Incorrect number entered!\n");
        }

        break;

      case 3:
        exit_cheak_case = 1;

        system("clear");
        if (PhonebookSearch(phonebook)) {
          printf(" Nothing found\n");
        }

        printf("\n\n 0 to Exit > ");
        while (exit_cheak_case) {
          scanf("%d", &exit_cheak_case);
          system("clear");
        }

        break;

      case 4:
        exit_cheak_case = 1;

        system("clear");
        PhonebookPrint(phonebook, entry_number);

        printf("\n\n 0 to Exit > ");
        while (exit_cheak_case) {
          scanf("%d", &exit_cheak_case);
          system("clear");
        }
        break;

      default:
        break;
    }
  }

  return 0;
}