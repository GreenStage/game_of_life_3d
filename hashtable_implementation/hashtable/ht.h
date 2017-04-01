#define HASH

#ifndef DEFS
	#include "../utils/defs.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct table_entry_{
  int *coords;
  State next_state;
  int generation; /*Generation in which the cell was created*/
  int visited; /*Generation in which the next_state was updated*/
  struct table_entry_ *next;
}Cell;

typedef struct hash_table_{
  int world_size;
  int table_size;
  struct table_entry_ **hashtable;
}World;

World * ht_initialize(int size, int world_size);

int ht_new_entry(World *ht, int generation, int x, int y, int z);

Cell* ht_find_entry(Cell **hashtable, int table_size, int *coords);

Cell* ht_remove_entry(Cell *head, Cell *rm, Cell *rm_prev);

void ht_clear(World *ht);
