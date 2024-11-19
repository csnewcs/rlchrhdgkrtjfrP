#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *my_reverse(char *originStr) {
  int len = strlen(originStr);
  char *stack = calloc(sizeof(char), (len + 1));
  int i = 0;
  for (; originStr[i]; i++) {
    stack[i] = originStr[len - i - 1];
  }
  return stack;
}
char **splitBySpace(char *originStr, int *outLength) {
  int len = strlen(originStr) - 1;
  char **split = calloc(sizeof(char *), (len + 1));
  int i = 1;
  split[0] = strtok(originStr, " ");
  for (; split[i] = strtok(NULL, " "); i++) {
    printf("Splited: %s\n", split[i]);
    fflush(stdout);
  }
  *outLength = i;
  return split;
}
void *input() {
  char *input = calloc(sizeof(char), 100);
  int i = 0;
  while ((input[i++] = getchar()) != '\n') {
  }
  input[i - 1] = 0;
  return (void *)input;
}

int out = 0;
typedef struct OutputArgs {
  char **output;
  int length;
} OutputArgs;
void *output(void *outputArgs) {
  OutputArgs args = *(OutputArgs *)outputArgs;
  int length = args.length - 1;
  char **output = args.output;
  int reversed = 0;
  for (int i = length; i >= 0; i--) {
    if (!strcmp(output[i], "Exit")) {
      printf("'Exit' detected, exiting...");
      out = 1;
      break;
    } else if (!strcmp(output[i], "Reverse")) {
      reversed = 1;
      printf("'Reverse' detected, after all strings will print reversed\n");
    }
    if (reversed) {
      output[i] = my_reverse(output[i]);
    }
    printf("Stack[%d]: %s\n", i, output[i]);
    sleep(1);
  }
}
int main() {
  pthread_t inputThread;
  pthread_t outputThread;
  char *inputStr;
  while (!out) {
    pthread_create(&inputThread, NULL, input, NULL);
    pthread_join(inputThread, (void **)&inputStr);
    if (inputStr[0] == 0) {
      printf("Any string isn't inputed. Exiting...");
      return 0;
    }
    printf("INPUT: %s", inputStr);
    int length;
    char **split = splitBySpace(inputStr, &length);
    OutputArgs outputArgs = {split, length};
    pthread_create(&outputThread, NULL, output, &outputArgs);
    pthread_join(outputThread, NULL);
  }
}
