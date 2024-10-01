#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *readFile(char fileName[]) {
  FILE *file = fopen(fileName, "r");
  static char content[100];
  if (file == NULL) {
    *content = '\0';
    return content;
  }
  char c = fgetc(file);
  for (int i = 0; c && i < 99; i++) {
    content[i] = c;
    c = fgetc(file);
  }
  fclose(file);
  remove(fileName);
  return content;
}
int main() {
  char *fileName = "./temp";
  for (;;) {
    char *r = readFile(fileName);
    if (strcmp(r, ""))
      printf("%s\n", r);
    usleep(10000); // 0.1sec
  }
  return 0;
}
