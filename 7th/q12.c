#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BRIDGE_AVAIL_WEIGHT 10
#define BRIDGE_QUEUE_SIZE 2
#define LINE_SIZE 2
typedef struct bridge_structure {
  int head; // Rear: Push point
  int tail; // Front: Pop point
  int avail_weight;
  int queue_size;
  int now_size;
  int *queue; // Real Queue Space
} BRIDGE;

// Init Structure
int *init_car_array(int total_car_count, int line);
BRIDGE *init_bridge(int total_car_count);
int sum_of_array(int *arr, int size);
// Queue Operations
int bridge_push(BRIDGE *bridge, int car_weight);
int bridge_pop(BRIDGE *bridge);
int bridge_get_weight(BRIDGE *bridge);
void *bridge_print(void *bridge);

// Main Operation
int pass_bridge(BRIDGE *bridge, int **car_arrays, int *car_counts);

// Thread
pthread_t printing_thread = 0;
typedef struct bridge_print_args {
  BRIDGE *bridge;
  char *tail;
} BRIDGE_PRINT_ARGS;
// ------------------------------------------------------------------------------------------

int main(void) {
  // Init the variants
  int car_counts[2];
  printf("input the number of first line's cars: ");
  scanf(" %d", &car_counts[0]);
  printf("input the number of second line's cars: ");
  scanf(" %d", &car_counts[1]);

  int **car_array = calloc(sizeof(int *), LINE_SIZE);
  for (int i = 0; i < LINE_SIZE; i++)
    car_array[i] = init_car_array(car_counts[i], i);

  BRIDGE *bridge = NULL;
  bridge = init_bridge(sum_of_array(car_counts, LINE_SIZE));

  // Print the status of time 0
  printf("(time 0) push - >> ");
  BRIDGE_PRINT_ARGS arg = {bridge, ""};
  bridge_print(&arg);
  printf("\n");

  // Pass the Bridge
  int time = 0;
  time = pass_bridge(bridge, car_array, car_counts);
  pthread_join(printing_thread, NULL);
  fflush(stdout);
  // printf("result time: %d\n", time);
  free(bridge->queue);
  free(bridge);
  free(car_array);
  printf("\n\n\n\n\n");
  return 0;
}

// ------------------------------------------------------------------------------------------

int *init_car_array(int total_car_count, int line) {
  int *temp;
  printf("input the weight of cars of line %d, line by line.\n", line);
  fflush(stdout);
  temp = (int *)calloc(total_car_count, sizeof(int));
  for (int i = 0; i < total_car_count; i++) {
    // printf("input the weight of car sequentially: ");
    scanf(" %d", &temp[i]);
  }

  return temp;
}

BRIDGE *init_bridge(int total_car_count) {
  BRIDGE *bridge = (BRIDGE *)malloc(sizeof(BRIDGE));

  bridge->head = 0; // Rear: Push point
  bridge->tail = 0; // Front: Pop point
  bridge->avail_weight = BRIDGE_AVAIL_WEIGHT;
  bridge->queue_size = BRIDGE_QUEUE_SIZE;
  bridge->now_size = 0;
  bridge->queue =
      (int *)calloc(bridge->queue_size, sizeof(int)); // Real Queue Space

  // Fill the bridge with dummy cars (0)
  for (int i = 0; i < bridge->queue_size; i++) {
    if (bridge_push(bridge, 0) == -1) {
      printf("[error] (init_bridge) push 0 dump fail!\n");
      assert(0);
    }
  }

  return bridge;
}
int sum_of_array(int *arr, int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }
  return sum;
}
// ---------------------------------------------------------------------------------------

int bridge_push(BRIDGE *bridge, int car_weight) {
  // Check Overflow
  if (bridge->now_size >= bridge->queue_size) {
    printf("[error] push to full queue!\n");
    return -1;
  }

  bridge->queue[bridge->head] = car_weight;
  bridge->head = (bridge->head + 1) % bridge->queue_size; // Circle Queue
  bridge->now_size++;

  return car_weight;
}

int bridge_pop(BRIDGE *bridge) {
  // Check Underflow
  if (bridge->now_size <= 0) {
    printf("[error] pop from empty queue!\n");
    return -1;
  }

  int temp = bridge->queue[bridge->tail];
  bridge->queue[bridge->tail] = 0;
  bridge->tail = (bridge->tail + 1) % bridge->queue_size; // Circle Queue
  bridge->now_size--;

  return temp;
}

int bridge_get_weight(BRIDGE *bridge) {
  int total_weight = 0;
  int temp_head = bridge->head;
  for (int i = 0; i < bridge->now_size + 1; i++) {
    total_weight += bridge->queue[temp_head];
    temp_head = (temp_head + 1) % bridge->queue_size;
  }

  // printf("[debug] total_weight : %d\n", total_weight);

  return total_weight;
}

void *bridge_print(void *bdg) {
  BRIDGE_PRINT_ARGS *args = (BRIDGE_PRINT_ARGS *)bdg;
  BRIDGE *bridge = args->bridge;
  int temp_head = bridge->head;
  temp_head = (temp_head + 1) % bridge->queue_size;
  printf("[ ");
  for (int i = 0; i < bridge->now_size; i++) {
    if (bridge->queue[temp_head] != 0)
      printf("%d ", bridge->queue[temp_head]);
    else
      printf("- ");
    temp_head = (temp_head + 1) % bridge->queue_size;
  }
  printf("] ");
  printf("%s", args->tail);
  printf("\n");
  fflush(stdout);
}

// --------------------------------------------------------------------------------------------------------------
char tail[400];
int pass_bridge(BRIDGE *bridge, int **car_arrays, int *car_array_count) {
  unsigned int time = 0;
  unsigned int *pushed_count = calloc(sizeof(int), LINE_SIZE);
  unsigned int popped_count = 0;
  int *reversed_index = calloc(sizeof(int), LINE_SIZE);
  for (int i = 0; i < LINE_SIZE; i++) {
    reversed_index[i] = car_array_count[i] - 1;
    pushed_count[i] = 0;
  }
  int temp = 0;
  int sum = sum_of_array(car_array_count, LINE_SIZE);
  while (popped_count < sum) {
    // 0. time up
    time++;
    if (printing_thread != 0)
      pthread_join(printing_thread, NULL);
    printf("(time %d) ", time);

    // 1. pop last car
    temp = bridge_pop(bridge);
    if (temp == -1)
      assert(0);
    else if (temp > 0)
      popped_count++;

    if (temp != 0) {
      sprintf(tail, ">> pop %d", temp);
    }
    // 2. check the weight
    // 2.1 if the weight is under, push next car 0
    // 2.2 if the weight is over, push the dummy car
    for (int i = 0; i < LINE_SIZE; i++) {
      reversed_index[i] = car_array_count[i] - pushed_count[i] - 1;
      if (car_arrays[i][reversed_index[i]] < 0)
        continue;
      if (bridge_get_weight(bridge) + car_arrays[i][reversed_index[i]] <=
              bridge->avail_weight &&
          reversed_index[i] >= 0) // Op 2.1 =>
      {

        int push_result = bridge_push(
            bridge, car_arrays[i][car_array_count[i] - pushed_count[i] - 1]);
        if (push_result == -1)
          assert(0);

        printf("push %d >> ",
               car_arrays[i][car_array_count[i] - pushed_count[i] - 1]);
        pushed_count[i]++;
        break;
      } else // Op 2.2 =>
      {

        if (i < LINE_SIZE - 1) { // if not last line
          continue;
        }
        if (bridge_push(bridge, 0) == -1)
          assert(0);
        printf("push - >> ");
      }
    }
    // 3. print the values
    pthread_t print_bridge;
    static BRIDGE_PRINT_ARGS arg;
    arg.bridge = bridge;
    arg.tail = tail;
    pthread_create(&print_bridge, NULL, *bridge_print, &arg);
    printing_thread = print_bridge;
    // usleep(1000);
  }

  return time;
}
