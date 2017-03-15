#include <stdlib.h>
#include <stdio.h>

typedef struct world_stct_ {
  int size;
  void * cell_list;
} world_stct;

world_stct * init_world( int size);

void world_map(world_stct * world);
