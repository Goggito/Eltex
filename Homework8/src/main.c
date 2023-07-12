#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/libinfo.h"

typedef struct {
  int func_number;
  int lib_number;
} FuncInfo;

int open_all_lib(char lib_name[100], void** lib_handler) {
  *lib_handler = dlopen(lib_name, RTLD_LAZY);
  if (*lib_handler == NULL) {
    fprintf(stderr, "Ошибка при загрузке библиотеки: %s\n", dlerror());
    return 1;
  }

  printf("Библиотека успешно загружена.\n");
  return 0;
}

int get_lib_info(void* lib_handler, LibInfo* lib_info) {
  typedef LibInfo (*func_type)();
  func_type get_info = (func_type)dlsym(lib_handler, "get_info");
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    printf("Ошибка при получении функции: %s\n", dlsym_error);
    dlclose(lib_handler);
    return 1;
  }

  *lib_info = get_info();
  return 0;
}

int get_func_info(void** lib_handler, FuncInfo* func_info, int* menu_counter,
                  int lib_quantity) {
  LibInfo lib_info;
  int i;
  int j;

  int f_num = 0;

  for (i = 0; i < lib_quantity; i++) {
    get_lib_info(&lib_handler[i], &lib_info);
    for (j = 0; j < lib_info.quantity_func; j++) {
      func_info[f_num].func_number = j;
      func_info[f_num].lib_number = i;
      f_num += 1;
    }
  }

  *menu_counter = i + 1;

  return 0;
}

int exec_func(int a, int b, int* result, void* lib_handler, int func_number) {
  LibInfo lib_info;
  get_lib_info(lib_handler, &lib_info);

  typedef int (*func_type)(int, int);
  func_type func =
      (func_type)dlsym(lib_handler, lib_info.func_real_name[func_number]);
  const char* dlsym_error = dlerror();

  if (dlsym_error) {
    printf("Ошибка при получении функции: %s\n", dlsym_error);
    dlclose(lib_handler);
    return 1;
  }

  *result = func(a, b);

  return 0;
}

int main() {
  char lib_name[10][100];
  void* lib_handler[10];
  int lib_quantity = 0;

  int menu_counter = 0;

  FuncInfo func_info[100];
  LibInfo lib_info[10];

  int option = 0;
  int exit_cheak = 0;
  int a = 0;
  int b = 0;
  int result = 0;

  int i;
  int j;

  // Запрос путей к библеотекам и их открытие
  printf("Введите кол-во библеотек: ");
  scanf("%d", &lib_quantity);

  for (int i = 0; i < lib_quantity; i++) {
    system("clear");
    printf("Введите имя библиотеки (полный путь к *.so): ");
    scanf("%s", lib_name[i]);

    void* current_handler;
    open_all_lib(lib_name[i], &current_handler);
    get_lib_info(current_handler, &lib_info[i]);
    lib_handler[i] = current_handler;
  }

  // Заполнение массивов с информацией для вызова функций

  int f_num = 0;

  for (i = 0; i < lib_quantity; i++) {
    for (j = 0; j < lib_info[i].quantity_func; j++) {
      func_info[f_num].func_number = j;
      func_info[f_num].lib_number = i;
      f_num += 1;
    }
  }
  menu_counter = i;

  printf("\n men %d   fun %d\n", menu_counter, f_num);

  system("clear");
  while (!exit_cheak) {
    system("clear");
    printf("\n     Calculator  \n");

    for (int i = 0; i < menu_counter; i++) {
      printf(" %d - %s \n", i + 1,
             lib_info[func_info[i].lib_number]
                 .func_menu_name[func_info[i].func_number]);
    }

    printf("\n 0 - Exit");
    printf("\n\n Result > %d", result);
    printf("\n\n > ");
    scanf("%d", &option);

    if (option == 0) {
      exit_cheak = 1;
    } else if (option > 0 && option <= menu_counter) {
      system("clear");
      printf("\n Enter A number:");
      scanf(" %d", &a);
      printf("\n Enter B number:");
      scanf(" %d", &b);
      exec_func(a, b, &result, lib_handler[func_info[option - 1].lib_number],
                func_info[option - 1].func_number);
    }
  }

  return 0;
}