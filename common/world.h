
typedef struct world_stct_ {
  int size;
  void * cell_ptr;
  void * (* add_cell)();

} world_stct;
