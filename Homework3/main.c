#include "stdio.h"

int ByteChange(int value, int byte_num, int replace) {
  unsigned char *ptr = &value;
  ptr += (byte_num - 1);
  *ptr = replace;

  return value;
}

int main() {
  int a = 0xDDCCBBAA;

  printf(" a == %X \n", a);

  unsigned char *c = &a;
  for (int i = 0; i < 4; i++) {
    printf(" Byte №%d == %X \n", i + 1, *c);
    c++;
  }

  // Меняем третий байт на 0xEE
  int b = ByteChange(a, 3, 0xEE);

  printf(" b == %X \n", b);
}