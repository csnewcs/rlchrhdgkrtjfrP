#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
void *printX() {
  while (1) {
    putchar('x');
    usleep(1000);
  }
}
int main() {
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, &printX, NULL);
  while (1) {
    putchar('o');
  }
}
