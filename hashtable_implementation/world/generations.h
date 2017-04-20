#define GEN

#ifndef HASH
  #include "../hashtable/ht.h"
#endif

/*Computes @world next generation. Marks cells that must die in the current
generation, @generation, and removes cells marked for death in the previous
generation. Updates current generation cell's next state*/
World * next_generations(World *world, int generation);

/*Returns dead or alive depending on the @current_state of the cell, and
the number of direct neighbours,  @neighbours*/
State cell_get_next_state(State current_state, int neighbours);

/*Fills the @coords vector with the coordinates relative to the current
position being analized given by @pos_index. Takes into accounte that
the world wraps around only returning coordinates between 0 and @world_size*/
void get_relative_coords(int pos_index, int world_size, int *coords);

/*Insertion sort algorithm that sorts living cells from @world using their
coordinates values as wheights. */ 
Cell ** sort_world(World *world);
