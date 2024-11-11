#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

typedef struct {
  char character;
  int count;
} charPrintParams;

void *printChar(void *params) {
  charPrintParams *p = (charPrintParams *)params;
  for (int i = 0; i < p->count; i++) {
    putchar(p->character);
  }
  return NULL;
}
int main() {
  pthread_t thread1Id;
  pthread_t thread2Id;
  charPrintParams thread1Args;
  charPrintParams thread2Args;

  thread1Args.character = 'x';
  thread1Args.count = 30000;
  pthread_create(&thread1Id, NULL, &printChar, &thread1Args);

  thread2Args.character = 'o';
  thread2Args.count = 30000;
  pthread_create(&thread2Id, NULL, &printChar, &thread2Args);
  int a;
  pthread_join(thread1Id, &a);
  pthread_join(thread2Id, &a);
  return 0;
}
