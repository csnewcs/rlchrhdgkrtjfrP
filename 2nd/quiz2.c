// UP DOWN GAME
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

void initGame(int chance, int *correctAnswer);
int setNumber(int min, int max);
void runGame(int correctAnswer, int chances);
int getAnswer();
int checkCorrect(int correctAnswer, int input);
void success(int correctAnswer);
void fail();

void main() {
  int rand;
  int leftChance = 5;
  initGame(leftChance, &rand);
  runGame(rand, leftChance);
}

void initGame(int chance, int *correctAnswer) {
  printf("Starting Game...\n");
  printf("You have %d chances.\n", chance);
  *correctAnswer = setNumber(1, 50);
  printf("%d\n", *correctAnswer);
}
int setNumber(int min, int max) { return min + (rand() % (max - min)); }

void runGame(int correctAnswer, int chances) {
  while (chances--) {
    int input = getAnswer();
    if (input > 50 || input < 1) {
      printf("Out of Range! You can input only 1~50. This not use chance");
      continue;
    }
    int upDown = checkCorrect(correctAnswer, input);
    if (!upDown) {
      success(correctAnswer);
      return;
    }
    upDown == 1 ? printf("Up\n") : printf("Down\n");
  }
  fail();
}
int getAnswer() {
  printf("Input your answer: ");
  int answer = 0;
  scanf(" %d", &answer);
  return answer;
}
int checkCorrect(int correctAnswer, int input) { // 0: correct, 1: Up, -1: Down
  if (correctAnswer == input)
    return 0;
  if (correctAnswer > input)
    return 1;
  return -1;
}
void success(int correctAnswer) {
  printf("You did it! The answer was %d", correctAnswer);
}
void fail() { printf("Fail"); }
