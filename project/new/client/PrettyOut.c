#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define CURSOR_UP "\033[A"
#define CURSOR_DOWN "\033[B"
#define CURSOR_FRONT "\033[C"
#define CURSOR_BACK "\033[D"
#define COLOR_RED "\x1b[31m"
#define COLOR_RESET "\x1b[0m"
#define PRINT_WITH_COLOR(COLOR, STRING)                                        \
  printf("%s%s%s", COLOR, STRING, COLOR_RESET)
#define CURSOR_SAVE() printf("\033[s")
#define CURSOR_RESTORE() printf("\033[u")

struct winsize w;

char *seperatorStr;
char *moveCursorToHome;
char *strMultiply(char *str, int num);
int printSpec(char *cpuName, char *gpuName,
              char* memSize); // 0: Succeed, 1: Failed

void initConsole() {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  printf("Console Size: (%d, %d)\n", w.ws_col, w.ws_row);
  seperatorStr = strMultiply("=", w.ws_col);
  moveCursorToHome = strMultiply(CURSOR_BACK, w.ws_col);
  printf("Status: Collecting Your System Info...\n");
  printf("%s\n", seperatorStr);
  printf("CPU:\nGPU:\nMemory:");
}
int printSpec(char *cpuName, char *gpuName, char* memSize) {
  printf(moveCursorToHome);
  printf(strMultiply(CURSOR_UP, 2));
  if (cpuName[0] == '\0') {
    PRINT_WITH_COLOR(COLOR_RED, "CPU: Could not get CPU information! Exit...");
    return 1;
  } else {
    printf("CPU: %s", cpuName);
  }
  fflush(stdout);
  printf(CURSOR_DOWN);
  printf(moveCursorToHome);
  if (gpuName[0] == '\0') {
    PRINT_WITH_COLOR(COLOR_RED,
                     "GPU: Could not get graphic information! Exit...");
    return 1;
  } else {
    printf("GPU: %s", gpuName);
  }
  fflush(stdout);
  printf(CURSOR_DOWN);
  printf(moveCursorToHome);
  if (memSize == 0) {
    PRINT_WITH_COLOR(COLOR_RED,
                     "Memory: Could not get memory information! Exit...");
    return 1;
  } else {
    printf("Memory: %sGB", memSize);
  }
  printf("\n%s\n", seperatorStr);
  return 0;
}
char *strMultiply(char *str, int num) {
  int strLen = strlen(str);
  char *mult = calloc(sizeof(char), strLen * num);
  for (int i = 0; i < num; i++) {
    strcpy((mult + (i * strLen)), str);
  }
  return mult;
}
