#ifndef WORLD_HEADER
#define WORLD_HEADER
#include "../common/defs.h"
#include "cell.h"

typedef struct border_{
  cell_ptr * cells;
  int owner;
} border;

struct world_stct {
  int sizeX, sizeY,sizeZ;
  int smallWorldLimits[4];
  border borders[4];

  int dim_size[2];
  int pID;
  int pNumb;
  int * rqstArray;
  MPI_Comm comm;
  MPI_Status status;
  /*Auxiliar variables*/
  int * zArray;

  cell_ptr ** cell_matrix;

  /*Functions*/

  /*******************************************************************
  * NAME :            world_destroy
  * TYPE :            void
  * DESCRIPTION :     destroys world structure and frees its memory
  *
  * INPUTS :   (GLOBAL) world_stct * world
  *
  * OUTPUTS :   (none)
  *******************************************************************/
  void (*destroy)();

  /*******************************************************************
  * NAME :            world_add_cell
  * TYPE :            void
  * DESCRIPTION :     adds a cell to world list at position (pos.x,pos.y)
  *
  * INPUTS :   (GLOBAL) world_stct * world
  *            pos_ pos;
  *
  * OUTPUTS :   (none)
  *******************************************************************/
  void (*add_cell)(pos_ pos);

  /*******************************************************************
  * NAME :            world_map
  * TYPE :            void
  * DESCRIPTION :     finds every neigbor for each cell inside world structure
  *
  * INPUTS :   (GLOBAL) world_stct * world
  *
  * OUTPUTS :   (none)
  *******************************************************************/
  void (*map)();

  /*******************************************************************
  * NAME :            world_get_next_gen
  * TYPE :            void
  * DESCRIPTION :     computes next_generation taking into account each cell neighbors
  *
  * INPUTS :   (GLOBAL) world_stct * world
  *
  * OUTPUTS :   (none)
  *******************************************************************/
  void (*get_next_gen)();

  /*******************************************************************
  * NAME :            world_update_gen
  * TYPE :            void
  * DESCRIPTION :     Updates generation to generation n+1
  *
  * INPUTS :   (GLOBAL) world_stct * world
  *
  * OUTPUTS :   (none)
  *******************************************************************/
  void (*update_gen)();

  /*******************************************************************
  * NAME :            world_order
  * TYPE :            void
  * DESCRIPTION :     orders every cell for each world position by (x,y,z)
  *
  * INPUTS :   (GLOBAL) world_stct * world
  *
  * OUTPUTS :   (none)
  *******************************************************************/
  void (*order)();

  /*******************************************************************
  * NAME :            world_print
  * TYPE :            void
  * DESCRIPTION :     Prints every alive cell in world structure.
  *
  * INPUTS :   (GLOBAL) world_stct * world
  *
  * OUTPUTS :   (none)
  *******************************************************************/
  void (*print)(FILE * file);
};
/*Define global pointer to world structure*/
struct world_stct * world;
/*******************************************************************
* NAME :            world_init
* TYPE :            void
* DESCRIPTION :     Initializes global world structure
*
* INPUTS :  int     size    world side size,
*           (GLOBAL) world_stct * world
*
* OUTPUTS :   (none)
*******************************************************************/

/*MPI FETCHING BORDERS*/
void fetch_borders();

void world_init( int sizeX, int sizeY, int sizeZ, int smallWorldLimits[4], int pid, MPI_Comm comm,int dim_size[2],int pNumb);
#endif
