#include "cell.h"

#define MINI_SIZE 4

typedef struct mini_world_{
  int size;
  cell_stct * center;
  struct mini_world_ * next;
  void (* world_destroy)();
} mini_world;

mini_world * miniworld_new();
void miniworld_destroy(mini_world * mini_world);
