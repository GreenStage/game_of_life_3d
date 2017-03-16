#include "world.h"

world_stct * init_world( int size){
  world_stct * new_world = (world_stct* ) malloc(sizeof(world_stct));
  new_world->size = size;
  new_world->cell_list = NULL;

  return new_world;
}

void world_map(void* cell_list){
  cell_stct *aux1, *aux2;
  Position pos;

  #ifdef DEBUG
    int counter = 0;
  #endif

    for(aux1 = (cell_stct*) cell_list; aux1 != NULL; aux1 = aux1->next){

  #ifdef DEBUG
      printf("Cell list pos %d\n", counter);
      counter++;
  #endif

    for(aux2 = aux1->next; aux2 != NULL; aux2 = aux2->next){
      pos = belongs_to_diamond(aux1->x, aux1->y, aux1->z, aux2->x, aux2->y, aux2->z);
      switch(pos){
        case 0:
          printf("NO NEIGHBOUR\n");
          break;
        case FRONT:
          aux1->first_neighbors[0] = aux2; /*FRONT*/
          aux2->first_neighbors[1] = aux1; /*BACK !!*/
          break;
        case BACK:
          aux1->first_neighbors[1] = aux2;
          aux2->first_neighbors[0] = aux1;
          reak;
        case RIGHT:
          aux1->first_neighbors[2] = aux2;
          aux2->first_neighbors[3] = aux1;
          break;
        case LEFT:
          aux1->first_neighbors[3] = aux2;
          aux2->first_neighbors[2] = aux1;
          break;
        case UP:
          aux1->first_neighbors[4] = aux2;
          aux2->first_neighbors[5] = aux1;
          break;
        case DOWN:
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

        default: error_exit("Error: Invalid Position when building World ", 18);
            break;
      }
    }//end second for
  }// end first for
}
