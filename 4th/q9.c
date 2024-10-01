#include <stdio.h>

FILE *readFile();

int countTxt[5] = {0};
int words = 0;
enum TextClass { lowercase, uppercase, newline, space, special };
char inputTxt[10000];
int main() {
  // intput.txt
  //  line number
  //  word count
  //  upper case count
  //  spcae count
  //  special keywords count
  FILE *file = readFile();
  printf(
      "line: %d\nwords: %d\nupper case: %d\nspace: %d\nspecial keyword: %d\n",
      countTxt[newline], words, countTxt[uppercase], countTxt[space],
      countTxt[special]);
  return 0;
}

FILE *readFile() {
  FILE *input = fopen("./input.txt", "r");
  int c; // char is not big enough to store EOF
  int plus = lowercase;
  int alphabetCount = 0;
  for (int i = 0; (c = fgetc(input)) != EOF; i++) {
    inputTxt[i] = c;
    plus = lowercase;
    if (c >= 'a' && c <= 'z')
      alphabetCount++;
    else if (c >= 'A' && c <= 'Z') {
      alphabetCount++;
      plus = uppercase;
    } else if (c == '\n')
      plus = newline;
    else if (c == ' ') {
      plus = space;
    } else
      plus = special;
    if ((c == '-' || plus == space || plus == newline) && alphabetCount != 0) {
      words++;
      alphabetCount = 0;
    }
    countTxt[plus]++;
  }
  return input;
}
