#include <stdlib.h>
#include <stdio.h>
#include "defs.h"
#include "error.h"
#include "cell.h"

typedef struct world_stct {
  int size;
  void * cell_list;
}world_stct;

world_stct * init_world( int size);

void world_map(void *cell_list);
