#include "Internet.h"
#include "Prettyout.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitCurl() {
  CURLcode res;
  curl_global_init(CURL_GLOBAL_DEFAULT);
}
extern char **gameListBuffer; // 게임 리스트 버퍼, 게임 선택할 때 사용
size_t makeGameListBuffer(char *data, size_t size, size_t nmamb,
                          void *clientp) {
  size_t realSize = size * nmamb;
  MemoryStruct *mem = (MemoryStruct *)clientp;

  char *res = realloc(mem->Response, mem->Size + realSize + 1);
  mem->Response = res;
  memcpy(&(mem->Response[mem->Size]), data, realSize);
  mem->Size += realSize;
  mem->Response[mem->Size] = 0;
  fprintf(stderr, "%s\n", mem->Response);
  extern int gameCount;
  for (int i = 0, j = 0; i < mem->Size; i++, j++) {
    if (mem->Response[i] == '\n') {
      gameCount++;
      j = -1;
      continue;
    }
    gameListBuffer[gameCount][j] = mem->Response[i];
  }
  extern int step;
  step = 1;

  return realSize;
}

void *performCurl(void *curl_handle) {
  CURLcode res = curl_easy_perform((CURL *)curl_handle);
  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
}

Request NewRequest(char *url, char *content, Header *headers,
                   enum Method method,
                   size_t (*callbackFunction)(char *, size_t, size_t, void *)) {
  convertUriFormat(url);
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
  MemoryStruct *body = malloc(sizeof(MemoryStruct));
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackFunction);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)body);
  pthread_create(&requestThread, NULL, performCurl, (void *)curl);
  Request req = {url, content, NULL, requestThread, method, body};
  return req;
}
void convertUriFormat(char *url) {
  char temp[600] = {0};
  for (int i = 0, j = 0; url[i]; i++, j++) {
    if (url[i] == ' ') {
      temp[j] = '%';
      temp[j + 1] = '2';
      temp[j + 2] = '0';
      j += 2;
      continue;
    }
    temp[j] = url[i];
  }
  strcpy(url, temp);
}
