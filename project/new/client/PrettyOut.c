#include "Prettyout.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================= 콘솔 크기 관련 변수 (initConsole에서 초기화)
// =================

char *seperatorStr;
char *moveCursorToHome;
char **consoleBuffer; // 출력할 콘솔 버퍼
char **gameListBuffer; 
struct winsize w;
struct termios term;

int step = 0;
int selectedGame = 0;
int gameCount = 0;
void initConsole() {
  step = 0;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  tcgetattr(STDIN_FILENO, &term);
  struct termios new_term = term;
  new_term.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
  printf("Console Size: (%d, %d)\n", w.ws_col, w.ws_row);
  seperatorStr = strMultiply("=", w.ws_col);
  moveCursorToHome = strMultiply(CURSOR_UP, w.ws_row);
  consoleBuffer = calloc(sizeof(char *), w.ws_row);
  gameListBuffer = calloc(sizeof(char *), w.ws_row);
  for (int i = 0; i < w.ws_row; i++) {
    consoleBuffer[i] = calloc(sizeof(char), w.ws_col);
    gameListBuffer[i] = calloc(sizeof(char), w.ws_col);
  }
  GET_CONSOLE();
  ERASE_CONSOLE();
  CURSOR_MOVE(1, 1);
  strcpy(consoleBuffer[0], "Status: Collecting Your System Info...");
  sprintf(consoleBuffer[1], "%s", seperatorStr);
  strcpy(consoleBuffer[2],"CPU:");
  strcpy(consoleBuffer[3], "GPU:");
  strcpy(consoleBuffer[4], "Memory:");
}


void *printThread(void *running) { // main에서 변수 변경하도록
  int *run = (int *)running;
  while (*run) {
    usleep(1000 * 100); //10fps
    ERASE_CONSOLE();
    CURSOR_MOVE(1, 1);
    if(step == 1) {
      printf("Selected Game: %d (Press arrow keys and ENTER to select)\n", selectedGame);
      for(int i = 0; gameListBuffer[i][0] != 0 && i < w.ws_row; i++) {
        i == selectedGame ? printf("%s%s%s%s\n",UNDERLINE, BOLD, gameListBuffer[i], RESET_ALL) : printf("%s\n", gameListBuffer[i]);
      }
      continue;
    }
    for (int i = 0; (consoleBuffer[i][0] != 0 && i < w.ws_row - 1); i++) {
      printf("%s\n", consoleBuffer[i]);
    }
  }
}

int printSpec(char *cpuName, char *gpuName, char *memSize) {
  if (cpuName[0] == '\0') {
    sprintf(consoleBuffer[2],"%sCPU: Could not get CPU information! Exit...%s", COLOR_RED, COLOR_RESET);
    return 1;
  } else {
    sprintf(consoleBuffer[2], "CPU: %s", cpuName);
  }
  fflush(stdout);
  printf(CURSOR_DOWN);
  printf(moveCursorToHome);
  if (gpuName[0] == '\0') {
    sprintf(consoleBuffer[3],"%sGPU: Could not get GPU information! Exit...%s", COLOR_RED, COLOR_RESET);
    return 1;
  } else {
    sprintf(consoleBuffer[3], "GPU: %s", gpuName);
  }
  fflush(stdout);
  printf(CURSOR_DOWN);
  printf(moveCursorToHome);
  if (memSize == 0) {
    sprintf(consoleBuffer[4],"%sMemory: Could not get Memory information! Exit...%s", COLOR_RED, COLOR_RESET);
    return 1;
  } else {
    sprintf(consoleBuffer[4], "Memory: %sGB", memSize);
  }
  sprintf(consoleBuffer[5], "%s", seperatorStr);
  strcpy(consoleBuffer[0], "Status: Getting game list from server...");
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
