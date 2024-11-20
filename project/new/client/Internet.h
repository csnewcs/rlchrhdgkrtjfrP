#include <curl/curl.h>
#include <curl/easy.h>
#include <pthread.h>
#include <sys/types.h>

enum Method { GET, POST };
typedef struct Header {
  char *Key;
  char *Value;
} Header;
typedef struct MemoryStruct {
  char *Response;
  size_t Size;
} MemoryStruct;
typedef struct Response {
  char *Content;
} Response;
typedef struct Request {
  char *Url;
  char *Content;
  struct Header *Headers;
  pthread_t RequestThread;
  enum Method Method;
  MemoryStruct *MemoryStruct;
} Request;
void InitCurl();
Request NewRequest(char *url, char *content, Header *headers,
                   enum Method method,
                   size_t (*callbackFunction)(char *, size_t, size_t, void *));
Response NewResponse(Request *request, char *content);
size_t makeGameListBuffer(char *, size_t, size_t, void *);
void convertUriFormat(char *);
