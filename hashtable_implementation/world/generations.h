#define GEN

#ifndef HASH
  #include "../hashtable/ht.h"
#endif

World * next_generations(World *world, int generation);

State cell_get_next_state(State current_state, int neighbours);

void get_relative_coords(int pos_index, int world_size, int *coords);
