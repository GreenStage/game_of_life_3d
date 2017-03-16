#ifndef WORLD_HEADER
#define WORLD_HEADER



typedef struct world_stct {
  int size;
  void * cell_list;
}world_stct;

world_stct * init_world( int size);

void world_map(void *cell_list);
#endif
