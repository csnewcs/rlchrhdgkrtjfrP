// Palindrome
#include <stdio.h>

void removeSpace(char *str);
int getStringSize(const char *str);
int isRepeated(char *str, int index, int strSize);

void main() {
  char input[10];
  scanf("%s", input);
  removeSpace(input);
  int size = getStringSize(input);
  isRepeated(input, 0, size) ? printf("TRUE") : printf("FALSE");
}

void removeSpace(char *str) {
  for (int i = 0; i < 10; i++) {
    if (str[i] == ' ') {
      for (int j = i; j < 9; j++) {
        str[j] = str[j + 1];
      }
    }
  }
}
int getStringSize(const char *str) {
  int i = 0;
  while (str[i]) {
    i++;
  }
  return i;
}
int isRepeated(char *str, int index, int strSize) {
  // ABCBA
  int next = 1;
  if (index < strSize / 2 - 1) {
    next = isRepeated(str, index + 1, strSize);
  }
  return str[index] == str[strSize - index - 1] && next;
}
