#include "stdio.h"

int ByteChange(int value, int byte_num, int replace) {
  int mask = 0xFFFFFFFF & ~(0xFF << 8 * (byte_num - 1));
  int result = (value & mask) | (replace << 8 * (byte_num - 1));

  return result;
}

int main() {
  int a = 0xDDCCBBAA;

  printf(" a == %X \n", a);

  int c;
  for (int i = 0; i < 4; i++) {
    c = ((a >> (8 * i)) & 0xFF);
    printf(" Byte №%d == %X \n", i + 1, c);
  }

  // Меняем третий байт на 0xEE
  int b = ByteChange(a, 3, 0xEE);

  printf(" b == %X \n", b);
}
