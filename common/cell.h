typedef struct cell_{
  int x,y,z;
  int visited;
  struct cell_ (*  first_neighbors)[6];
  struct cell_ (* second_neighbors)[18];
  enum {dead,alive} state;
  enum {dead,alive} next_state;
  struct cell_ * next;
} cell_stct;

cell lst * new_cell_list();
cell_stct * new_cell( cell_stct * ptr,int x , int y, int z);
