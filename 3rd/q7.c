#include <stdio.h>

// int realNumbers[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
const int numbers[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int getN();
void getTwoNumbers(int *, int *);
void printNumbers(int *);
void swap(int *, int, int);
int main() {
  int n = getN();
  int ptrArray[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (int i = 0; i < n; i++) {
    int a, b;
    getTwoNumbers(&a, &b);
    swap(ptrArray, a, b);
    printNumbers(ptrArray);
  }
}
int getN() {
  int inputNumber = -1;
  printf("N 입력: ");
  scanf(" %d", &inputNumber);
  return inputNumber;
}
void getTwoNumbers(int *a, int *b) {
  printf("두 개의 값을 입력하세요(입력 양식: a b) ");
  scanf(" %d %d", a, b);
  if ((*a > 9 || *a < 0) || (*b > 9 || *b < 0)) {
    printf("잘못된 값! 두 숫자는 0부터 9 사이에 있어야 합니다\n");
    getTwoNumbers(a, b);
  }
}
void printNumbers(int *a) {
  putchar('\n');
  for (int i = 0; i < 10; i++) {
    printf("%d ", numbers[a[i]]);
  }
}

void swap(int *arr, int a, int b) {
  int temp = arr[a];
  arr[a] = arr[b];
  arr[b] = temp;
}
