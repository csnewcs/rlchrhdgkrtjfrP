#include <stdio.h>
#include <stdlib.h>
#define MAX_WEIGHT 10
#define LENGTH 2

typedef struct {
  int Weight;
  int StartTime;
  int EndTime;
} Car;

Car bridge[2];
Car cars[20];
void CarQueuePush(Car);
Car CarQueuePop();
int carQueueFront = -1;
int carQueueRear = -1;

void BridgePrint();
int shiftBridge(Car);
Car lastPopFromBridgeCar;

void cycle();

void PrintNow();
Car NullCar = {0, -1, -1};
int time = 0;

int main() {
  int carCount;
  int *inputCars = calloc(carCount, sizeof(int));
  printf("Input the number of cars: ");
  scanf("%d", &carCount);
  for (int i = 0; i < carCount; i++) {
    int weight;
    printf("Input %d car's weight: ", i);
    scanf(" %d", &weight);
    inputCars[i] = weight;
  }
  // init
  bridge[0] = bridge[1] = lastPopFromBridgeCar = NullCar;
  for (int i = 0; i < carCount; i++) {
    Car c = {inputCars[i], 0, 0};
    CarQueuePush(c);
    printf("%d car pushed(Rear: %d)\n", c.Weight, carQueueRear);
  }
  while (!(carQueueFront == carQueueRear && bridge[0].Weight == 0 &&
           bridge[1].Weight == 0)) {
    cycle();
  }
}

void cycle() {
  time++;

  Car next = CarQueuePop();
  if (shiftBridge(next)) {
  } else {
    carQueueFront--;
  }
  PrintNow();
}
int shiftBridge(Car newCar) {
  lastPopFromBridgeCar = bridge[1];
  bridge[1] = bridge[0];
  if (lastPopFromBridgeCar.Weight != 0) {
    lastPopFromBridgeCar.EndTime = time;
  }
  if (bridge[1].Weight + newCar.Weight > MAX_WEIGHT) {
    bridge[0] = NullCar;
    return 0;
  }
  newCar.StartTime = time;
  bridge[0] = newCar;
  return 1;
}
void PrintNow() {
  printf("(time %d) push  ", time);
  bridge[0].Weight == 0 ? printf("- >> [ ")
                        : printf("%d >> [ ", bridge[0].Weight);
  BridgePrint();
  printf("]");
  if (lastPopFromBridgeCar.StartTime != -1) {
    printf(">> pop %d", lastPopFromBridgeCar.Weight);
  }
  printf("\n");
}
void BridgePrint() {
  bridge[0].Weight > 0 ? printf("%d ", bridge[0].Weight) : printf("- ");
  bridge[1].Weight > 0 ? printf("%d ", bridge[1].Weight) : printf("- ");
};

void CarQueuePush(Car car) {
  if (carQueueRear == 20 - 1)
    return;
  cars[++carQueueRear] = car;
}
Car CarQueuePop() {
  if (carQueueFront == carQueueRear)
    return NullCar;
  return cars[++carQueueFront];
}
