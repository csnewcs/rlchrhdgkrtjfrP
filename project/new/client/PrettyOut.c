#include "Prettyout.h"
#include "GetInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
  strcpy(consoleBuffer[1], seperatorStr);
  strcpy(consoleBuffer[2], "CPU:");
  strcpy(consoleBuffer[3], "GPU:");
  strcpy(consoleBuffer[4], "Memory:");
}

void *printThread(void *running) { // main에서 변수 변경하도록
  int *run = (int *)running;
  while (*run) {
    usleep(1000 * 100); // 10fps
    setStatusStr();
    ERASE_CONSOLE();
    CURSOR_MOVE(1, 1);
    if (step == 1) {
      printf("Selected Game: %d (Press arrow keys and ENTER to select)\n",
             selectedGame + 1);
      for (int i = 0; gameListBuffer[i][0] != 0 && i < w.ws_row; i++) {
        i == selectedGame ? printf("%s%s%s%s\n", UNDERLINE, BOLD,
                                   gameListBuffer[i], RESET_ALL)
                          : printf("%s\n", gameListBuffer[i]);
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
    sprintf(consoleBuffer[2], "%sCPU: Could not get CPU information! Exit...%s",
            COLOR_RED, COLOR_RESET);
    return 1;
  } else {
    sprintf(consoleBuffer[2], "CPU: %s", cpuName);
  }
  if (gpuName[0] == '\0') {
    sprintf(consoleBuffer[3], "%sGPU: Could not get GPU information! Exit...%s",
            COLOR_RED, COLOR_RESET);
    return 1;
  } else {
    sprintf(consoleBuffer[3], "GPU: %s", gpuName);
  }
  if (memSize == 0) {
    sprintf(consoleBuffer[4],
            "%sMemory: Could not get Memory information! Exit...%s", COLOR_RED,
            COLOR_RESET);
    return 1;
  } else {
    sprintf(consoleBuffer[4], "Memory: %sGB", memSize);
  }
  sprintf(consoleBuffer[5], "%s", seperatorStr);
  return 0;
}

int printedRequirements = 0;
int printRequirements(
    char *serverResponse, int part,
    Info
        info) { // 양식:
                // 실행가능성\n최소CPU,최소GPU,최소메모리\n권장CPU,권장GPU,권장메모리
                // / 0: CPU, 1: GPU, 2: Memory
  printedRequirements++;

  fprintf(stderr, "%d: %s Started\n", printedRequirements, info.infoName);
  char token = serverResponse[0];
  if (token == '-') {
    sprintf(consoleBuffer[6 + part], "%s%s: %s is not found in the server!%s",
            COLOR_PURPLE, info.infoName, info.info, COLOR_RESET);
  } else {
    if (!strcmp(info.infoName, "CPU"))
      fprintf(stderr, "csv\n%s\n", serverResponse);
    char ***csv = csvParsing(serverResponse);

    if (token == '0') {
      sprintf(consoleBuffer[6 + part], "%s%s: Cannot run (%s < %s)%s",
              COLOR_RED, info.infoName, info.info, csv[1][part], COLOR_RESET);
    } else if (token == '1') {
      sprintf(consoleBuffer[6 + part], "%s%s: Not Recommended (%s < %s)%s",
              COLOR_YELLOW, info.infoName, info.info, csv[2][part],
              COLOR_RESET);
    } else if (token == '2') {
      sprintf(consoleBuffer[6 + part], "%s%s: OK! (%s > %s)%s", COLOR_BLUE,
              info.infoName, info.info, csv[2][part], COLOR_RESET);
    }
    fprintf(stderr, "%s: strcpy done!\n", info.infoName);
  }
  if (printedRequirements == 3) {
    step = 3;
  }
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
void setStatusStr() {
  switch (step) {
  case 0:
    strcpy(consoleBuffer[0], "Status: Collecting your system info...");
    break;
  case 1:
    strcpy(consoleBuffer[0], "Status: Selecting game"); // NOT SHOWN
    break;
  case 2:
    strcpy(consoleBuffer[0], "Status: Checking if your system meets the "
                             "requirements of the selected game");
    break;
  case 3:
    strcpy(consoleBuffer[0], "Status: DONE! Press 'q' to exit");
    break;
  }
}
char ***csvParsing(char *originalStr) {
  /*
   * 1
   *  MinimumCPU,MinimumGpu,MinimumMem
   *  RecommendCPU,RecommendGPU,RecommendMem
   * */
  char ***csv = malloc(sizeof(char **) * 3);
  for (int i = 0; i < 3; i++) {
    csv[i] = malloc(sizeof(char *) * 3);
    for (int j = 0; j < 3; j++) {
      csv[i][j] = calloc(sizeof(char), 100);
    }
  }

  char temp[100] = {0};
  int j = 0, row = 0, column = 0;
  for (int i = 0; originalStr[i]; i++, j++) {
    if (originalStr[i] == ',') {
      temp[j] = 0;
      j = -1;
      strcpy(csv[row][column], temp);
      column++;
    } else if (originalStr[i] == '\n') {
      temp[j] = 0;
      j = -1;
      strcpy(csv[row][column], temp);
      column = 0;
      row++;
    } else {
      temp[j] = originalStr[i];
    }
  }
  temp[j] = 0;
  strcpy(csv[row][column], temp);
  return csv;
}
