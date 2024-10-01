#include <stdio.h>

void main() {
  char *text = "안녕하세요";
  for (int i = 0; i < 15; i++) {
    printf("%2X\n", text[i]);
    fflush(stdout);
  }
}
