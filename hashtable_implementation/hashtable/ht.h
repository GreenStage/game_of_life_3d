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
	int live_cells;
  struct table_entry_ **hashtable;
}World;

/*Initializes a new World structure*/
World * ht_initialize(int size, int world_size);

/*Initializes a new hashtable entry and adds it to World @ht hashtable.
Returns 1 if there is a collision and 0 otherwise*/
int ht_new_entry(World *ht, int generation, int x, int y, int z);

/*Returns pointer to the cell at the position specified by @coords.
Returns NULL if the cell is not on the list*/
Cell* ht_find_entry(Cell **hashtable, int table_size, int *coords);

/*Removes the cell pointed by @rm from the list and frees it's
allocated memory. Returns a pointer to the head of the list updated
by this function*/
Cell* ht_remove_entry(Cell *head, Cell *rm, Cell *rm_prev);

void ht_clear(World *ht);
