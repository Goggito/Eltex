#include "../include/mul.h"

int quantity_func = 1;
char func_real_name[MAX_FUNC_NUMB][LEN_FUNC_NAME] = {{"my_mul"}};
char func_menu_name[MAX_FUNC_NUMB][LEN_FUNC_NAME] = {{"*"}};

LibInfo get_info() {
  LibInfo info;
  int i, j;

  info.quantity_func = quantity_func;
  for (i = 0; i < MAX_FUNC_NUMB; i++) {
    for (j = 0; j < LEN_FUNC_NAME; j++) {
      info.func_real_name[i][j] = func_real_name[i][j];
      info.func_menu_name[i][j] = func_menu_name[i][j];
    }
  }

  return info;
}

int my_mul(int a, int b) { return a * b; }