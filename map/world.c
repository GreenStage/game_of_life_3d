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

cell_stct * world_map(cell_stct * cell_list, int runs){
  cell_stct * aux1, *aux2, *jump, *prev = NULL;
  Position pos;
  int i;

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
      if(runs != 1)
        aux1->state = alive;

      if(aux1->visited == 1){
        aux1->visited = 0;
        prev = aux1;
        continue;
      }

      for(aux2 = aux1->next; aux2 != NULL; aux2 = aux2->next){
        pos = belongs_to_diamond(aux1->x, aux1->y, aux1->z, aux2->x, aux2->y, aux2->z);
        set_neighbour(aux1, aux2, pos);

#ifdef DEBUG
        if( pos != 0) printf("Cell (%d,%d,%d) is <%d> of cell (%d,%d,%d)\n",aux2->x,aux2->y,aux2->z,pos,aux1->x,aux1->y,aux1->z);
#endif
      }

      if(runs == 1)
        aux1->visited = 1;

      prev = aux1;

    } else{
      if(runs != 1){

        for(i = 0; i < 6; i++)
          if(aux1->first_neighbors[i] != NULL){
            pos = belongs_to_diamond(aux1->x, aux1->y, aux1->z, aux1->first_neighbors[i]->x, aux1->first_neighbors[i]->y, aux1->first_neighbors[i]->z);
            set_neighbour(NULL, aux1->first_neighbors[i], pos);
          }

        for(i = 0; i < 18; i++)
          if(aux1->second_neighbors[i] != NULL){
            pos = belongs_to_diamond(aux1->x, aux1->y, aux1->z, aux1->second_neighbors[i]->x, aux1->second_neighbors[i]->y, aux1->second_neighbors[i]->z);
            set_neighbour(NULL, aux1->second_neighbors[i], pos);
          }

        if(aux1 != cell_list)
          prev->next = aux1->next;
        else
          cell_list = aux1->next;


        free(aux1->first_neighbors);
        free(aux1->second_neighbors);
        free(aux1);
        aux1 = NULL;

#ifdef DEBUG
        cell_stct *auxx;
        if(cell_list != NULL)
          for(auxx = cell_list; auxx != NULL; auxx = auxx->next)
              printf("FINAL %d %d %d\n", auxx->x, auxx->y, auxx->z);
#endif
      }
    }
    if(runs == 1)
      break;
  }

  return cell_list;
}

cell_stct * insert_new_cell(cell_stct *list, State st, int x , int y, int z){
  cell_stct *new_cell = (cell_stct*) malloc(sizeof(cell_stct));
  memset(new_cell, 0, sizeof(cell_stct));

  new_cell->x = x;
  new_cell->y = y;
  new_cell->z = z;
  new_cell->state = st;
  new_cell->next_state = alive; /*default alive*/
  new_cell->visited = 0;

  new_cell->first_neighbors = (cell_stct**) malloc(6*sizeof(cell_stct*));
  memset(new_cell->first_neighbors, 0, 6*sizeof(cell_stct*));

  new_cell->second_neighbors  = (cell_stct**) malloc(18*sizeof(cell_stct*));
  memset(new_cell->second_neighbors, 0, 18*sizeof(cell_stct*));

  new_cell->next = list;

  return new_cell;
}

cell_stct * next_world_gen(cell_stct *list){
  cell_stct *aux1;
  int i, j, first_neighbors = 0, second_neighbors = 0, second_neighbor_index[5];
  int coords[3] = {0};

  for(aux1 = list; aux1 != NULL; aux1 = aux1->next){

#ifdef DEBUG
      int counter = 0;
      printf("Cell list processing at next_word_gen() position %d <%d %d %d>\n", counter, aux1->x, aux1->y, aux1->z);
      counter++;
#endif

    first_neighbors = 0;
    for(i = 0; i < 6; i++){
      if(aux1->first_neighbors[i] == NULL){
        get_neighbours_by_key(second_neighbor_index, i);
        second_neighbors = 0;
        for(j = 0; j < 5; j++)
          if(aux1->second_neighbors[second_neighbor_index[j]] != NULL && aux1->second_neighbors[second_neighbor_index[j]]->state != dead)
            second_neighbors++;


        if(cell_get_next_state(dead, second_neighbors + 1 /*current cell*/) == alive){
          coords[0] = aux1->x; coords[1] = aux1->y; coords[2] = aux1->z;
          generate_coords(coords, i);
          list = insert_new_cell(list, dead, coords[0], coords[1], coords[2]); /*returns pointer to new cell*/
        /*  printf("new cell at <%d, %d, %d>\n\n", list->x, list->y, list->z);*/
          world_map(list, 1);
          /*printf("\n");*/
        }
      } else{
        if(aux1->first_neighbors[i]->state != dead)
          first_neighbors++;
      }
    }

    aux1->next_state = cell_get_next_state(aux1->state, first_neighbors);
  }
  return list;
}

void generate_coords(int *coords, int pos_index){
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
  }

Position belongs_to_diamond(int xcomp, int ycomp, int zcomp, int x, int y, int z){
  int distance;
  Position pos;
  int coord_dif[3] = {x - xcomp, y - ycomp, z - zcomp};

  if((distance = abs(coord_dif[0]) + abs(coord_dif[1]) + abs(coord_dif[2])) == 1 || distance == 2)
    pos = get_neighbour_pos(distance, coord_dif);
  else
    pos = NONE;

  return pos;
}

void set_neighbour(cell_stct *current, cell_stct *neighbour, Position pos){

  switch(pos){
    case NONE:
      break;

    case FRONT:
      if(current != NULL)
        current->first_neighbors[0] = neighbour;
      neighbour->first_neighbors[1] = current;
      break;

    case BACK:
      if(current != NULL)
        current->first_neighbors[1] = neighbour;
      neighbour->first_neighbors[0] = current;
      break;

    case RIGHT:
      if(current != NULL)
        current->first_neighbors[2] = neighbour;
      neighbour->first_neighbors[3] = current;
      break;

    case LEFT:
      if(current != NULL)
        current->first_neighbors[3] = neighbour;
      neighbour->first_neighbors[2] = current;
      break;

    case UP:
      if(current != NULL)
        current->first_neighbors[4] = neighbour;
      neighbour->first_neighbors[5] = current;
      break;

    case DOWN:
      if(current != NULL)
        current->first_neighbors[5] = neighbour;
      neighbour->first_neighbors[4] = current;
      break;

    case U1_L1:
      if(current != NULL)
      current->second_neighbors[0] = neighbour;
      neighbour->second_neighbors[5] = current;
      break;

    case U1_R1:
    if(current != NULL)
      current->second_neighbors[1] = neighbour;
      neighbour->second_neighbors[4] = current;
      break;

    case U1_F1:
    if(current != NULL)
      current->second_neighbors[2] = neighbour;
      neighbour->second_neighbors[7] = current;
      break;

    case U1_B1:
    if(current != NULL)
      current->second_neighbors[3] = neighbour;
      neighbour->second_neighbors[6] = current;
      break;

    case D1_L1:
    if(current != NULL)
      current->second_neighbors[4] = neighbour;
      neighbour->second_neighbors[1] = current;
      break;

    case D1_R1:
    if(current != NULL)
      current->second_neighbors[5] = neighbour;
      neighbour->second_neighbors[0] = current;
      break;

    case D1_F1:
    if(current != NULL)
      current->second_neighbors[6] = neighbour;
      neighbour->second_neighbors[3] = current;
      break;

    case D1_B1:
    if(current != NULL)
      current->second_neighbors[7] = neighbour;
      neighbour->second_neighbors[2] = current;
      break;

    case B1_L1:
    if(current != NULL)
      current->second_neighbors[8] = neighbour;
      neighbour->second_neighbors[11] = current;
      break;

    case B1_R1:
    if(current != NULL)
      current->second_neighbors[9] = neighbour;
      neighbour->second_neighbors[10] = current;
      break;

    case F1_L1:
    if(current != NULL)
      current->second_neighbors[10] = neighbour;
      neighbour->second_neighbors[9] = current;
      break;

    case F1_R1:
    if(current != NULL)
      current->second_neighbors[11] = neighbour;
      neighbour->second_neighbors[8] = current;
      break;

    case F2:
    if(current != NULL)
      current->second_neighbors[12] = neighbour;
      neighbour->second_neighbors[13] = current;
      break;

    case B2:
    if(current != NULL)
      current->second_neighbors[13] = neighbour;
      neighbour->second_neighbors[12] = current;
      break;

    case R2:
    if(current != NULL)
      current->second_neighbors[14] = neighbour;
      neighbour->second_neighbors[15] = current;
      break;

    case L2:
    if(current != NULL)
      current->second_neighbors[15] = neighbour;
      neighbour->second_neighbors[14] = current;
      break;

    case U2:
    if(current != NULL)
      current->second_neighbors[16] = neighbour;
      neighbour->second_neighbors[17] = current;
      break;
    case D2:
    if(current != NULL)
      current->second_neighbors[17] = neighbour;
      neighbour->second_neighbors[16] = current;
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

void clear_map(cell_stct *list){
  cell_stct *aux1, *aux2;

  for(aux1 = list; aux1 != NULL; aux1 = aux2){
    aux2 = aux1->next;
    free(aux1->first_neighbors);
    free(aux1->second_neighbors);
    free(aux1);
  }
}
