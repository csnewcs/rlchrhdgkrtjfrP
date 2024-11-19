#include "Internet.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <pthread.h>

void InitCurl() {
  CURLcode res;
  curl_global_init(CURL_GLOBAL_DEFAULT);
}
void *performCurl(void *curl_handle) {
  CURLcode res = curl_easy_perform((CURL *)curl_handle);
  printf("Stat: %d\n", res);
}

Request NewRequest(char *url, char *content, Header *headers,
                   enum Method method) {
  CURL *curl;
  curl = curl_easy_init();
  printf("URL: %s, Method: %d\n", url, method);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  switch (method) {
  case GET:
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    break;
  case POST:
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
    break;
  }
  pthread_t requestThread;
  pthread_create(&requestThread, NULL, performCurl, (void *)curl);
  Request req = {url, content, NULL, requestThread, method};
  return req;
}
