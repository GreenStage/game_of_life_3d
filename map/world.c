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

cell_stct * world_map(cell_stct * cell_list, int runs, int world_size){
  cell_stct * aux1, *aux2, *jump, *prev = NULL;
  Position pos, pos_nb;
  int i, j, coords[3] = {0}, second_neighbor_index[5] = {0}, second_neighbors, first_neighbors;

#ifdef DEBUG
      int counter = 0;
#endif

  for(aux1 = cell_list; aux1 != NULL ; aux1 = jump){
    jump = aux1->next;
#ifdef DEBUG
      printf("Cell list processing at world_map() position %d <%d %d %d>\n", counter, aux1->x, aux1->y, aux1->z);
      counter++;
#endif

    if(aux1->next_state == alive){

      for(aux2 = cell_list; aux2 != NULL; aux2 = aux2->next){
        if(aux2->next_state == alive){ /*No need to check for neighbours in cells that are going to die*/
          memset(coords, 0, 3*sizeof(int));
          get_comp_coords(0, world_size, aux1->x, aux1->y, aux1->z, aux2->x, aux2->y, aux2->z, coords);
          pos = belongs_to_diamond(coords);
          set_neighbour(aux1, aux2, pos);

#ifdef DEBUG
        if( pos != 0) printf("Cell (%d,%d,%d) is <%d> of cell (%d,%d,%d)\n",aux2->x,aux2->y,aux2->z,pos,aux1->x,aux1->y,aux1->z);
#endif
          if(aux1->near_border != 0 && aux2->near_border != 0){ /*Only check for near border conditions if both cells have the near_border flag set*/
            memset(coords, 0, 3*sizeof(int));
            get_comp_coords(1, world_size, aux1->x, aux1->y, aux1->z, aux2->x, aux2->y, aux2->z, coords);
            pos_nb = belongs_to_diamond(coords);
            if(pos_nb != pos)
              set_neighbour(aux1, aux2, pos_nb);
#ifdef DEBUG
        if( pos_nb != 0 && pos_nb != pos) printf("Near border cell (%d,%d,%d) is <%d> of cell (%d,%d,%d)\n",aux2->x,aux2->y,aux2->z,pos_nb,aux1->x,aux1->y,aux1->z);
#endif
          }
        }
      }

      first_neighbors_count = 0;
      for(i = 0; i < 6; i++){
        if(first_neighbors[i] == NULL){
          get_neighbours_by_key(second_neighbor_index, i);
          second_neighbors_count = 0;
          for(j = 0; j < 5; j++)
            if(second_neighbors[second_neighbor_index[j]] != NULL && second_neighbors[second_neighbor_index[j]]->state != dead)
              second_neighbors_count++;

          if(cell_get_next_state(dead, second_neighbors + 1 /*current cell*/) == alive){
            coords[0] = aux1->x; coords[1] = aux1->y; coords[2] = aux1->z;
            generate_relative_coords(i, coords, world_size);
            cell_list = insert_new_cell(list, dead, coords[0], coords[1], coords[2], world_size); /*returns pointer to new cell*/
          }

        } else{
          if(first_neighbors[i]->state != dead)
            first_neighbors_count++;
        }
      }

      aux1->next_state = cell_get_next_state(aux1->state, first_neighbors_count);
      prev = aux1;

    } else{

        if(aux1 != cell_list)
          prev->next = aux1->next;
        else
          cell_list = aux1->next;

        free(aux1);
        aux1 = NULL;
    }
  }

  return cell_list;
}

void generate_relative_coords(int pos_index, int *coords, int world_size){
  int i;

  switch(pos_index){

      case 0: /*Front*/
        coords[0]++;
        break;

      case 1:
        coords[0]--;
        break;

      case 2:
         coords[1]++;
        break;

      case 3:
         coords[1]--;
        break;

      case 4:
         coords[2]++;
        break;

      case 5:
      default:
         coords[2]--;
        break;

    }


    for(i = 0; i < 3; i++){
      if(coords[i] < 0)
        coords[i] = world_size + coords[i];
      if(coords[i] >= world_size)
        coords[i] = coords[i] - world_size;
    }
  }

Position belongs_to_diamond(int *coord_dif){
  int distance;
  Position pos;

  #ifdef DEBUG
    printf("coord_dif : [%d,%d,%d]\n",coord_dif[0],coord_dif[1],coord_dif[2]);
  #endif
  if((distance = abs(coord_dif[0]) + abs(coord_dif[1]) + abs(coord_dif[2])) == 1 || distance == 2)
    pos = get_neighbour_pos(distance, coord_dif);
  else
    pos = NONE;

  return pos;
}

void get_comp_coords(int near_border, int world_size, int xcomp, int ycomp, int zcomp, int x, int y, int z, int *coords){
  int aux;

  coords[0] = x - xcomp;
  coords[1] = y - ycomp;
  coords[2] = z - zcomp;

  if(near_border == 1){

    if(coords[0] != 0){
      if((aux = x - world_size) >= -2)
        if(xcomp < 2)
          coords[0] = aux - xcomp;
      if(x < 2)
        if((aux = xcomp - world_size) >= -2)
          coords[0] = abs(aux) + x;
    }

    if(coords[1] != 0){
      if((aux = y - world_size) >= -2)
        if(ycomp < 2)
          coords[1] = aux - ycomp;
      if(y < 2)
        if((aux = ycomp - world_size) >= -2)
          coords[1] = abs(aux) + y;
    }

    if(coords[2] != 0){
      if((aux = z - world_size) >= -2)
        if(zcomp < 2)
          coords[2] = aux - zcomp;
      if(z < 2)
        if((aux = zcomp - world_size) >= -2)
          coords[2] = abs(aux) + z;
    }
  }

}

void set_neighbour(cell_stct *current, cell_stct *neighbour, Position pos){

  switch(pos){
    case NONE:
      break;

    case FRONT:
      if(current != NULL)
      first_neighbors[0] = neighbour;
      first_neighbors[1] = current;
      break;

    case BACK:
      if(current != NULL)
      first_neighbors[1] = neighbour;
      first_neighbors[0] = current;
      break;

    case RIGHT:
      if(current != NULL)
      first_neighbors[2] = neighbour;
      first_neighbors[3] = current;
      break;

    case LEFT:
      if(current != NULL)
      first_neighbors[3] = neighbour;
      first_neighbors[2] = current;
      break;

    case UP:
      if(current != NULL)
      first_neighbors[4] = neighbour;
      first_neighbors[5] = current;
      break;

    case DOWN:
      if(current != NULL)
      first_neighbors[5] = neighbour;
      first_neighbors[4] = current;
      break;

    case U1_L1:
      if(current != NULL)
      second_neighbors[0] = neighbour;
      second_neighbors[5] = current;
      break;

    case U1_R1:
    if(current != NULL)
    second_neighbors[1] = neighbour;
    second_neighbors[4] = current;
      break;

    case U1_F1:
    if(current != NULL)
      second_neighbors[2] = neighbour;
      second_neighbors[7] = current;
      break;

    case U1_B1:
    if(current != NULL)
      second_neighbors[3] = neighbour;
      second_neighbors[6] = current;
      break;

    case D1_L1:
    if(current != NULL)
      second_neighbors[4] = neighbour;
      second_neighbors[1] = current;
      break;

    case D1_R1:
    if(current != NULL)
      second_neighbors[5] = neighbour;
      second_neighbors[0] = current;
      break;

    case D1_F1:
    if(current != NULL)
      second_neighbors[6] = neighbour;
      second_neighbors[3] = current;
      break;

    case D1_B1:
    if(current != NULL)
      second_neighbors[7] = neighbour;
      second_neighbors[2] = current;
      break;

    case B1_L1:
    if(current != NULL)
      second_neighbors[8] = neighbour;
      second_neighbors[11] = current;
      break;

    case B1_R1:
    if(current != NULL)
      second_neighbors[9] = neighbour;
      second_neighbors[10] = current;
      break;

    case F1_L1:
    if(current != NULL)
      second_neighbors[10] = neighbour;
      second_neighbors[9] = current;
      break;

    case F1_R1:
    if(current != NULL)
      second_neighbors[11] = neighbour;
      second_neighbors[8] = current;
      break;

    case F2:
    if(current != NULL)
      second_neighbors[12] = neighbour;
      second_neighbors[13] = current;
      break;

    case B2:
    if(current != NULL)
      second_neighbors[13] = neighbour;
      second_neighbors[12] = current;
      break;

    case R2:
    if(current != NULL)
      second_neighbors[14] = neighbour;
      second_neighbors[15] = current;
      break;

    case L2:
    if(current != NULL)
      second_neighbors[15] = neighbour;
      second_neighbors[14] = current;
      break;

    case U2:
    if(current != NULL)
      second_neighbors[16] = neighbour;
      second_neighbors[17] = current;
      break;
    case D2:
    if(current != NULL)
      second_neighbors[17] = neighbour;
      second_neighbors[16] = current;
      break;

    default:
#ifdef DEBUG
      printf("ERROR NOW: Cell (%d,%d,%d) is <%d> of cell (%d,%d,%d)\n", neighbour->x, neighbour->y, neighbour->z, pos, current->x, current->y, current->z);
#endif
      error_exit("Error: Invalid Position when building World ()", 18);
      break;
  }

}

Position get_neighbour_pos(int distance, int coord_dif[3]){
  int is_negative = 0, i;
  Position pos = NONE;

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

void clear_map(cell_stct *list){
  cell_stct *aux1, *aux2;

  for(aux1 = list; aux1 != NULL; aux1 = aux2){
    aux2 = aux1->next;
    free(aux1->first_neighbors);
    free(aux1->second_neighbors);
    free(aux1);
  }
}
