#include <stdio.h>

enum { KRW = 1, JPY = 928, USD = 1328 };

void getMode(int *start, int *end);
int getMoney();
int exchange(int start, int end, int money);
char *formatting(int number);
int main() {
  int start, end;
  getMode(&start, &end);
  int after = exchange(start, end, getMoney());
  printf("[test] after exchange: %d\n", after);
  printf("%s", formatting(after));
}
void getMode(int *start, int *end) {
  printf("모드를 선택하세요\n1. KRW to USD\n2. KRW to JPY\n3. JPY to USD\n4. "
         "USD to JPY\n----------------\n");
  int mode = 0;
  scanf(" %d", &mode);
  if (mode < 1 || mode > 4) {
    printf("잘못된 입력! 1번부처 4번까지 중 선택하세요!\n");
    getMode(start, end);
  }
  if (mode == 1) {
    *start = KRW, *end = USD;
  } else if (mode == 2) {
    *start = KRW, *end = JPY;
  } else if (mode == 3) {
    *start = JPY, *end = USD;
  } else {
    *start = USD, *end = JPY;
  }
}
int getMoney() {
  int inputNumber = -1;
  printf("금액 입력: ");
  scanf(" %d", &inputNumber);
  return inputNumber;
}
int exchange(int start, int end, int money) { return (money * start) / end; }
char *formatting(int number) {
  static char str[100];
  int digit = 1;
  int tempNumber = number;
  int strSize = 0;
  int thousand = 1000;
  while (tempNumber) {
    digit *= 10;
    strSize++;
    if (digit / thousand == 1) {
      strSize++;
      thousand *= 1000;
    }
    tempNumber /= 10;
    if (!tempNumber)
      break;
  }
  digit /= 10;
  thousand /= 1000;
  tempNumber = number;
  int first = 1;
  for (int i = 0; i < strSize; i++) {
    tempNumber = number % digit;
    printf("digit: %d, number: %d, tempNumber: %d, thousand: %d\n", digit,
           number / digit, tempNumber, thousand);
    str[i] = 48 + (number / digit);
    if (digit / thousand == 1 && thousand != 1) {
      str[++i] = ',';
      // printf("added comma!\n");
      thousand /= 1000;
    }
    if (thousand / digit == 10 && first) {
      str[++i] = ',';
      first = 0;
    }
    digit /= 10;
    number = tempNumber;
  }
  return str;
}
