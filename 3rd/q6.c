#include <stdio.h>

// 입력 받은 정수에서 0~9 개수 세고, 이를 내림차순 출력
// 처음: 몇 개의 정수 'N'을 받음
// N개 정수 입력
// 입력 받은걸 내림차순 정렬
int getInputSize();
int getInputNumber();
int *getOrder(int *);
void printArr(int *);

int main() {
  int inputSize = getInputSize();
  if (inputSize == 0)
    return 0;
  int inputNumbers[10] = {0}; // 0~9
  for (int i = 0; i < inputSize; i++) {
    inputNumbers[getInputNumber()]++;
  }
  int *order = getOrder(inputNumbers); // SortedList 급구!
  // printArr(order);
}
int getInputSize() {
  int inputSize = 0;
  printf("몇 개의 수를 받을까요? ");
  scanf("%d", &inputSize);
  if (inputSize > 30000 || inputSize < 0) {
    printf("잘못된 크기(크기는 0~30000 범위 안에 있어야 합니다!)\n");
    return getInputSize();
  }
  return inputSize;
}
int getInputNumber() {
  int inputNumber = -1;
  printf("숫자 입력: ");
  scanf(" %d", &inputNumber);
  if (inputNumber > 9 || inputNumber < 1) {
    printf("잘못된 숫자(숫자는 0~9 범위 안에 있어야 합니다!)\n");
    return getInputNumber();
  }
  return inputNumber;
}
int *getOrder(int *array) {
  static int order[10] = {-1};
  for (int i = 0; i < 10; i++) {
    int maxIndex = 0;
    for (int j = 0; j < 10; j++) {
      if (array[maxIndex] < array[j]) {
        maxIndex = j;
      }
    }
    order[i] = maxIndex;
    if (array[maxIndex] != 0) {
      printf("%d: %d\n", maxIndex, array[maxIndex]);
    }
    array[maxIndex] = -1;
  }
  return order;
}
void printArr(int *array) {
  for (int i = 0; i < 10; i++) {
    printf("%d\n", array[i]);
  }
}
