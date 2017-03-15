typedef struct cell_{
int x,y,z;
int first_neighbors[6];
int second_neighbors[18];
} cell_stct;

typdef struct cell_tmp_{
  cell_stct * ptr;
  struct cell_tmp_ * next;
} cell_tmp;
