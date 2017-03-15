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
  int counter = 0;

  for(aux1 = (cell_stct*) cell_list; aux1 != NULL; aux1 = aux1->next){
    printf("counter %d\n", counter);
    counter++;
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
            break;
        case LEFT:
            break;
        case RIGHT:
          printf("RIGHT\n");
          break;
        case UP:
            break;
        case DOWN:
          break;
        case F2:
          break;
        case B2:
            break;
        case L2:
            break;
        case R2:
          printf("R2\n");
          break;
        case U2:
            printf("U2");
            break;
        case D2:
            break;
        case U1_L1:
          break;
        case U1_R1:
            break;
        case U1_F1:
            break;
        case U1_B1:
          break;
        case D1_L1:
          break;
        case D1_R1:
            break;
        case D1_F1:
            break;
        case D1_B1:
          break;
        case B1_L1:
            break;
        case B1_R1:
            break;
        case F1_L1:
          break;
        case F1_R1:
            break;
        default: error_exit("Error: Position switch entered 'defaul'", 18);
            break;
      }
    }//end second for
  }// end first for
}
