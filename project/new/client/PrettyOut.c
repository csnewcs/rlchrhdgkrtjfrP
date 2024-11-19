#include "Prettyout.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// ================= 콘솔 크기 관련 변수 (initConsole에서 초기화)
// =================
struct winsize w;
struct termios term;
char *seperatorStr;
char *moveCursorToHome;
char **consoleBuffer; // 출력할 콘솔 버퍼
// ================= 기능 함수 헤더 =================
char *strMultiply(char *str, int num);
int printSpec(char *cpuName, char *gpuName,
              char *memSize); // 0: Succeed, 1: Failed

void initConsole() {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  tcgetattr(STDIN_FILENO, &term);
  struct termios new_term = term;
  new_term.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
  printf("Console Size: (%d, %d)\n", w.ws_col, w.ws_row);
  seperatorStr = strMultiply("=", w.ws_col);
  moveCursorToHome = strMultiply(CURSOR_UP, w.ws_row);
  consoleBuffer = calloc(sizeof(char *), w.ws_row);
  for (int i = 0; i < w.ws_row; i++) {
    consoleBuffer[i] = calloc(sizeof(char), w.ws_col);
  }
  GET_CONSOLE();
  ERASE_CONSOLE();
  CURSOR_MOVE(1, 1);
  printf("Status: Collecting Your System Info...\n");
  printf("%s\n", seperatorStr);
  printf("CPU:\nGPU:\nMemory:");
}

void *printThread(void *running) { // main에서 변수 변경하도록
  int *run = (int *)running;
  int i = 0;
  while (*run) {
    sleep(1);
    ERASE_CONSOLE();
    CURSOR_MOVE(1, 1);
    printf("%d", i++);
    for (int i = 0; i < w.ws_row; i++) {
      printf("%s\n", consoleBuffer[i]);
    }
  }
}

int printSpec(char *cpuName, char *gpuName, char *memSize) {
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
