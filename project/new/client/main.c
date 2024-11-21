// CPU 이름 가져오기: /proc/cpuinfo의 model name 부분
// 메모리 용량 가져오기: free --giga -h 에서 Mem: 부분 중 첫 번째(total)
// GPU 이름 가져오기: lshw -C display에서 product 부분"들" 중 대괄호 안 (바깥은
// GPU 코어 이름, NAVI14와 같은)

#include "./GetInfo.h"
#include "./Internet.h"
#include "./Prettyout.h"
#include <memory.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
void *mainFuncThread(); // main함수는 사용자의 key입력(q 등)을 받고 그간 다른
                        // 작업은 mainFuncThread에서 수행
size_t cpuCallback(char *, size_t, size_t, void *);
size_t gpuCallback(char *, size_t, size_t, void *);
size_t memCallback(char *, size_t, size_t, void *);

Info cpuInfo;
Info gpuInfo;
Info memInfo;

int main() {
  extern int step;
  extern int selectedGame;
  extern int gameCount;
  initConsole();
  InitCurl();
  int printRunning = 1;
  pthread_t print;
  pthread_t mainFunc;
  pthread_create(&print, NULL, printThread, &printRunning);
  pthread_create(&mainFunc, NULL, mainFuncThread, NULL);
  char input;
  while (1) {
    input = getchar();
    if (input == 'q') {
      break;
    } else if (input == '\033' && step == 1) {
      getchar(); // skip [
      input = getchar();
      if (input == 'A') { // UP
        if (selectedGame > 0)
          selectedGame--;
      } else if (input == 'B') { // DOWN
        if (selectedGame < gameCount) {
          selectedGame++;
        }
      }
    } else if ((input == '\n' || input == '\r') && step == 1) {
      step = 2;
    }
  }
  printRunning = 0;
  pthread_join(print, NULL);
  printf("q inputed... Exiting\n");
  pthread_cancel(mainFunc);
  sleep(3);
  RELEASE_CONSOLE();
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
void *mainFuncThread() {
  // step 0
  cpuInfo = GetCpuInfo();
  gpuInfo = GetGpuInfo();
  memInfo = GetMemoryInfo();
  if (printSpec(cpuInfo.info, "TEST GPU", memInfo.info)) {
    fprintf(stdin, "q");
  }
  // step 1
  const char *SERVER_URL = "http://kakao.csnewcs.dev:8080";
  char gameListUrl[200];
  char gameCpuUrl[200];
  char gameGpuUrl[200];
  char gameMemUrl[200];

  sprintf(gameListUrl, "%s/gamelist", SERVER_URL);
  printf("Game List URL: %s\n",
         gameListUrl); // 이거 없으니까 이유는 모르겠으나 아래 NewRequest에
                       // 잘못된 URL을 전송... 혹시 sprintf 비동기인가?
  Request req = NewRequest(gameListUrl, "", NULL, GET, makeGameListBuffer);
  pthread_join(req.RequestThread, NULL);
  sprintf(gameCpuUrl, "%s/cpucheck/%d?cpu=%s", SERVER_URL, selectedGame + 1,
          cpuInfo.info);
  sprintf(gameGpuUrl, "%s/gpucheck/%d?gpu=%s", SERVER_URL, selectedGame + 1,
          gpuInfo.info);
  sprintf(gameMemUrl, "%s/memcheck/%d?mem=%s", SERVER_URL, selectedGame + 1,
          memInfo.info);

  while (step != 2) {
    usleep(1000); // 1ms
  }
  // step 2
  setupRequirements();
  Request cpuScore = NewRequest(gameCpuUrl, "", NULL, GET, cpuCallback);
  Request gpuScore = NewRequest(gameGpuUrl, "", NULL, GET, gpuCallback);
  Request memScore = NewRequest(gameMemUrl, "", NULL, GET, memCallback);
  pthread_join(cpuScore.RequestThread, NULL);
  pthread_join(gpuScore.RequestThread, NULL);
  pthread_join(memScore.RequestThread, NULL);
  sleep(1000);
  return 0;
}

size_t cpuCallback(char *data, size_t size, size_t nmamb, void *clientp) {
  size_t realSize = size * nmamb;
  MemoryStruct *mem = (MemoryStruct *)clientp;
  char *res = realloc(mem->Response, mem->Size + realSize + 1);
  mem->Response = res;
  memcpy(&(mem->Response[mem->Size]), data, realSize);
  mem->Size += realSize;
  mem->Response[mem->Size] = 0;
  printRequirements(mem->Response, 0, cpuInfo);
  return realSize;
}
size_t gpuCallback(char *data, size_t size, size_t nmamb, void *clientp) {
  size_t realSize = size * nmamb;
  MemoryStruct *mem = (MemoryStruct *)clientp;
  char *res = realloc(mem->Response, mem->Size + realSize + 1);
  mem->Response = res;
  memcpy(&(mem->Response[mem->Size]), data, realSize);
  mem->Size += realSize;
  mem->Response[mem->Size] = 0;
  printRequirements(mem->Response, 1, gpuInfo);
  return realSize;
}
size_t memCallback(char *data, size_t size, size_t nmamb, void *clientp) {
  size_t realSize = size * nmamb;
  MemoryStruct *mem = (MemoryStruct *)clientp;
  char *res = realloc(mem->Response, mem->Size + realSize + 1);
  mem->Response = res;
  memcpy(&(mem->Response[mem->Size]), data, realSize);
  mem->Size += realSize;
  mem->Response[mem->Size] = 0;
  printRequirements(mem->Response, 2, memInfo);
  return realSize;
}
