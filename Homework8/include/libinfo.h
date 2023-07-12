#ifndef LIBINFO_H
#define LIBINFO_H

#define MAX_FUNC_NUMB 10
#define LEN_FUNC_NAME 25

typedef struct {
  int quantity_func;
  char func_real_name[MAX_FUNC_NUMB][LEN_FUNC_NAME];
  char func_menu_name[MAX_FUNC_NUMB][LEN_FUNC_NAME];
} LibInfo;

#endif
