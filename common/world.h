#include "cell.h"

typedef struct world_{
  int size;
  cell_stct * alive[LLIST_SIZE];
  void (* world_destroy)();
} world_stct;

world_stct * world_init();
void world_destroy(world_stct * world);
