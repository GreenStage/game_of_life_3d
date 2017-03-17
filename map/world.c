#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "world.h"
#include "../common/error.h"

world_stct * init_world( int size){
  world_stct * new_world = (world_stct* ) malloc(sizeof(world_stct));
  new_world->size = size;
  new_world->cell_list = NULL;

  return new_world;
}

void world_map(cell_stct * cell_list){
  int first_neighbors_ctr, i, j  ;
  int neighbor_neigh[6], count_n_neigh;
  cell_stct * aux1, *aux2;
  Position pos;

  #ifdef DEBUG
    int counter = 0;
  #endif

    for(aux1 = cell_list; aux1 != NULL; aux1 = aux1->next){
      first_neighbors_ctr = 0;
#ifdef DEBUG
      printf("Cell list processing at position %d\n", counter);
      counter++;
#endif

    for(aux2 = aux1->next; aux2 != NULL; aux2 = aux2->next){
      pos = belongs_to_diamond(aux1->x, aux1->y, aux1->z, aux2->x, aux2->y, aux2->z);
      switch(pos){

        case NONE:
          break;

        case FRONT:
          first_neighbors_ctr++;
          aux1->first_neighbors[0] = aux2;
          aux2->first_neighbors[1] = aux1;
          break;

        case BACK:
          first_neighbors_ctr++;
          aux1->first_neighbors[1] = aux2;
          aux2->first_neighbors[0] = aux1;
          break;

        case RIGHT:
          aux1->first_neighbors[2] = aux2;
          aux2->first_neighbors[3] = aux1;
          break;

        case LEFT:
          first_neighbors_ctr++;
          aux1->first_neighbors[3] = aux2;
          aux2->first_neighbors[2] = aux1;
          break;

        case UP:
          first_neighbors_ctr++;
          aux1->first_neighbors[4] = aux2;
          aux2->first_neighbors[5] = aux1;
          break;

        case DOWN:
          first_neighbors_ctr++;
          aux1->first_neighbors[5] = aux2;
          aux2->first_neighbors[4] = aux1;
          break;

        case U1_L1:
          aux1->second_neighbors[0] = aux2;
          aux2->second_neighbors[1] = aux1;
          break;

        case U1_R1:
          aux1->second_neighbors[1] = aux2;
          aux2->second_neighbors[0] = aux1;
          break;

        case U1_F1:
          aux1->second_neighbors[2] = aux2;
          aux2->second_neighbors[3] = aux1;
          break;

        case U1_B1:
          aux1->second_neighbors[3] = aux2;
          aux2->second_neighbors[2] = aux1;
          break;

        case D1_L1:
          aux1->second_neighbors[4] = aux2;
          aux2->second_neighbors[5] = aux1;
          break;

        case D1_R1:
          aux1->second_neighbors[5] = aux2;
          aux2->second_neighbors[4] = aux1;
          break;

        case D1_F1:
          aux1->second_neighbors[6] = aux2;
          aux2->second_neighbors[7] = aux1;
          break;

        case D1_B1:
          aux1->second_neighbors[7] = aux2;
          aux2->second_neighbors[6] = aux1;
          break;

        case B1_L1:
          aux1->second_neighbors[8] = aux2;
          aux2->second_neighbors[9] = aux1;
          break;

        case B1_R1:
          aux1->second_neighbors[9] = aux2;
          aux2->second_neighbors[8] = aux1;
          break;


        case F1_L1:
          aux1->second_neighbors[10] = aux2;
          aux2->second_neighbors[11] = aux1;
          break;

        case F1_R1:
          aux1->second_neighbors[11] = aux2;
          aux2->second_neighbors[10] = aux1;
          break;

        case F2:
          aux1->second_neighbors[12] = aux2;
          aux2->second_neighbors[13] = aux1;
          break;

        case B2:
          aux1->second_neighbors[13] = aux2;
          aux2->second_neighbors[12] = aux1;
          break;

        case R2:
          aux1->second_neighbors[14] = aux2;
          aux2->second_neighbors[15] = aux1;
          break;

        case L2:
          aux1->second_neighbors[15] = aux2;
          aux2->second_neighbors[14] = aux1;
          break;

        case U2:
          aux1->second_neighbors[16] = aux2;
          aux2->second_neighbors[17] = aux1;
          break;
        case D2:
          aux1->second_neighbors[17] = aux2;
          aux2->second_neighbors[16] = aux1;
          break;

        default:
#ifdef DEBUG
          printf("ERROR NOW: Cell (%d,%d,%d) is <%d> of cell (%d,%d,%d)\n",aux2->x,aux2->y,aux2->z,pos,aux1->x,aux1->y,aux1->z);
#endif
          error_exit("Error: Invalid Position when building World ()", 18);
          break;
      }
#ifdef DEBUG
        if( pos != 0) printf("Cell (%d,%d,%d) is <%d> of cell (%d,%d,%d)\n",aux2->x,aux2->y,aux2->z,pos,aux1->x,aux1->y,aux1->z);
#endif
    }
  aux1->next_state = cell_get_next_state(aux1->state,first_neighbors_ctr);

    for(i=0;i<6;i++){
      count_n_neigh=0;
      if(aux1->first_neighbors[i] == NULL){
        get_neighbors_by_key(neighbor_neigh,i);
        for(j = 0; j < 5; j++){
          if(aux1->second_neighbors[neighbor_neigh[j]] != NULL)
            count_n_neigh++;
        }
        if( count_n_neigh > 0 )
          aux1->first_neighbors[i]->next_state = cell_get_next_state(aux1->first_neighbors[i]->state,count_n_neigh);
      }
    }
  }

}


Position belongs_to_diamond(int xcomp, int ycomp, int zcomp, int x, int y, int z){
  int distance;
  Position pos = NONE;
  int coord_dif[3] = {x - xcomp, y - ycomp, z - zcomp};

  if((distance = abs(coord_dif[0]) + abs(coord_dif[1]) + abs(coord_dif[2])) == 1 || distance == 2)
    pos = get_neighbour_pos(distance, coord_dif);
  else
    pos = NONE;

  return pos;
}

Position get_neighbour_pos(int distance, int coord_dif[3]){
  int is_negative = 0, i;
  Position pos = NONE;

#ifdef DEBUG
  printf("coord_dif : [%d,%d,%d] , ",coord_dif[0],coord_dif[1],coord_dif[2]);
#endif

  for(i = 0; i< 3; i++){
    is_negative = (coord_dif[i] < 0) ? 2 : 0;

    if(coord_dif[i] == 2 || coord_dif[i] == -2){
      pos = (0b11 << (i * 4 + is_negative));
      break;
    }
    else if(coord_dif[i] == 1 || coord_dif[i] == -1){
      pos = pos | (0b01 << (i * 4 + is_negative));
      if(distance == 1)
        break;
    }
    else continue;
  }

  return pos;
}
