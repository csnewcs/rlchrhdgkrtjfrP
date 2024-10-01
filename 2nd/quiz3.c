// leap year
#include <stdio.h>

int getYear();
int isLeapYear(int year);
void main() {
  int year = getYear();
  isLeapYear(year) ? printf("Yes. %d is leap year.\n", year)
                   : printf("No. %d isn't leap year.", year);
}
int getYear() {
  printf("Input number to check Leap Year: ");
  int year;
  scanf(" %d", &year);
  if (year < 1 || year > 3000) {
    printf("Out of range! You inputted more than 3000 or less than 1.");
    return getYear();
  }
  return year;
}
int isLeapYear(int year) {
  if (year % 400 == 0)
    return 1;
  if (year % 100 == 0)
    return 0;
  if (year % 4 == 0)
    return 1;
  return 0;
}
