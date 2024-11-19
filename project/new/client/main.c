// CPU 이름 가져오기: /proc/cpuinfo의 model name 부분
// 메모리 용량 가져오기: free --giga -h 에서 Mem: 부분 중 첫 번째(total)
// GPU 이름 가져오기: lshw -C display에서 product 부분"들" 중 대괄호 안 (바깥은
// GPU 코어 이름, NAVI14와 같은)

#include "./GetInfo.c"
#include "./Internet.h"
#include "./PrettyOut.c"
#include <pthread.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void *mainFuncThread(); // main함수는 사용자의 key입력(q 등)을 받고 그간 다른
                        // 작업은 mainFuncThread에서 수행
int main() {
  initConsole();
  InitCurl();
  int printRunning = 1;
  pthread_t print;
  pthread_t mainFunc;
  // pthread_create(&print, NULL, printThread, &printRunning);
  // pthread_create(&mainFunc, NULL, mainFuncThread, NULL);
  Request req = NewRequest("http://localhost:8080/2", "", NULL, GET);
  pthread_join(req.RequestThread, NULL);
  char input;
  while (1) {
    input = getchar();
    if (input == 'q') {
      break;
    }
  }
  printRunning = 0;
  pthread_join(print, NULL);
  printf("q inputed... Exiting\n");
  pthread_join(mainFunc, NULL);
  sleep(3);
  RELEASE_CONSOLE();
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
void *mainFuncThread() {
  Info cpuInfo = GetCpuInfo();
  Info gpuInfo = GetGpuInfo();
  Info memInfo = GetMemoryInfo();
  if (printSpec(cpuInfo.info, "TEST GPU", memInfo.info)) {
    FILE *fp = fopen(STDIN_FILENO, "w");
    fprintf(fp, "q");
  }
  return 0;
}
