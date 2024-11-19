#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *my_reverse(char *originStr) {
  int len = strlen(originStr);
  char *stack = malloc(sizeof(char) * (len + 1));
  int i = 0;
  for (; originStr[i]; i++) {
    printf("[%d]: %c\n", i, originStr[len - i]);
    stack[i] = originStr[len - i];
  }
  stack[i] = 0;
  return stack;
}
void main(int argc, char *argv[]) {
  printf("input: %s\n", argv[1]);
  printf("reverse: %s\n", my_reverse(argv[1]));
  printf("input: %s\n", argv[1]);
}
