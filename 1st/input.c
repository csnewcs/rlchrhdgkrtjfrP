#include <stdio.h>
#include <string.h>

void writeFile(char *fileName, char content[]) {
  if (content == "" || content == "\n")
    return;
  FILE *file = fopen(fileName, "w");
  fprintf(file, "%s", content);
  fclose(file);
}
void main() {
  char *fileName = "./temp";
  char content[100];
  for (;;) {
    printf("Insert string(99 characters limit): ");
    if (fgets(content, sizeof content, stdin))
      content[strcspn(content, "\n")] = '\0';
    writeFile(fileName, content);
  }
}
