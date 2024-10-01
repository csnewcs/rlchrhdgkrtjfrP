// Kasar
#include <stdio.h>

// ABCDEFGHIJKLMNOPQRSTUVWXYZ
char *round(char *, int);
char *getOriginalString();
int getKey();
void toUpperCase(char *str);

void main() {
  char *originalText = getOriginalString();
  toUpperCase(originalText);
  int key = getKey();
  char *afterRound = round(originalText, key);
  printf("Encoded Text: %s", afterRound);
}
char *getOriginalString() {
  static char str[11];
  printf("Input original string(10 characters limit): ");
  scanf("%s", str);
  return str;
}

void toUpperCase(char *str) {
  for (int i = 0; i < 10; i++) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      str[i] -= ('a' - 'A');
    }
  }
}
int getKey() {
  int key;
  printf("Input Key: ");
  scanf(" %d", &key);
  return key;
}
char *round(char *original, int key) {
  static char round[11];
  for (int i = 0; i < 11 && original[i] != '\0' && original[i] != '\n'; i++) {
    // 65~90
    round[i] = ((original[i] - 'A' + key) % 26) + 'A';
  }
  return round;
}
