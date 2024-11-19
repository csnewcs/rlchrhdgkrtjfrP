#include <curl/curl.h>
#include <curl/easy.h>
#include <pthread.h>

enum Method { GET, POST };
typedef struct Header {
  char *Key;
  char *Value;
} Header;

typedef struct Response {
  char *Content;
} Response;
typedef struct Request {
  char *Url;
  char *Content;
  struct Header *Headers;
  pthread_t RequestThread;
  enum Method Method;
} Request;
void InitCurl();
Request NewRequest(char *url, char *content, Header *headers,
                   enum Method method);
Response NewResponse(Request *request, char *content);
