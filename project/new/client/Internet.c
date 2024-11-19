#include "Internet.h"
#include "Prettyout.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct MemoryStruct {
  char *Response;
  size_t Size;
} MemoryStruct;

void InitCurl() {
  CURLcode res;
  curl_global_init(CURL_GLOBAL_DEFAULT);
}
extern char **gameListBuffer; // 게임 리스트 버퍼, 게임 선택할 때 사용
void curlCallback(char *data, size_t size, size_t nmamb, void *clientp) {
  size_t realSize = size * nmamb;
  MemoryStruct *mem = (MemoryStruct *)clientp;
  char* res = realloc(mem->Response, mem->Size + realSize + 1);
  mem->Response = res;
  memcpy(&(mem->Response[mem->Size]), data, realSize);
  mem->Size += realSize;
  mem->Response[mem->Size] = 0;

  extern int gameCount;
  for(int i = 0, j = 0; i < mem->Size; i++, j++) {
    if(mem->Response[i] == '\n') {
      gameCount++;
      j = -1;
      continue;
    }
    gameListBuffer[gameCount][j] = mem->Response[i];
  }
  printf("%cDONE!\n", gameListBuffer[1][0]);
  extern int step;
  step = 1;
}
void *performCurl(void *curl_handle) {
  static MemoryStruct body = {0};
  curl_easy_setopt((CURL *)curl_handle, CURLOPT_WRITEFUNCTION, curlCallback);
  curl_easy_setopt((CURL *)curl_handle, CURLOPT_WRITEDATA, (void *)&body);
  CURLcode res = curl_easy_perform((CURL *)curl_handle);
}

Request NewRequest(char *url, char *content, Header *headers,
                   enum Method method) {
  CURL *curl;
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url);
  switch (method) {
  case GET:
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    break;
  case POST:
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, 1);
    break;
  }
  pthread_t requestThread;
  pthread_create(&requestThread, NULL, performCurl, (void *)curl);
  Request req = {url, content, NULL, requestThread, method};
  return req;
}
