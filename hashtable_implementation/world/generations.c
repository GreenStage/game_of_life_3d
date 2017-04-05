#include "generations.h"

World * next_generations(World *world, int generation){
  int i, coords[3] = {0}, coords1[3] = {0}, neighbour_1i, neighbour_2i, neighbour_1count, neighbour_2count;
  int live_cells = 0;
  Cell *aux, *prev, *jump, *aux1, *aux2;

  /*Open MP notes: parallelize outter loop;
  private(prev, aux, neighbour_1i, neighbour_2i, neighbour_1count, neighbour_2count),
  private(aux1, aux2, jump),
  ??right command?? reduce(live_cells, add) or atomic(live_cells),#pragme omp atomic ensures that a specific storage location is updated
  atomically, rather than exposing it to the possibility of multiple,
  simultaneous writing threads.
  shared(world).
  Problems:
  how to solve load balancing; hashtable isn't uniformly filled,
  what happens when some thread writes on the hashtable?? ; every thread needs to see changes made before proceeding --
  to analize next cell; locks, #pragma omp critical;
  */
  for(i = 0; i < world->table_size; i++){

    prev = NULL;
    for(aux = world->hashtable[i]; aux != NULL; aux = jump){

      jump = aux->next;
      if(aux->next_state == alive && aux->generation != generation){ /*Only analize cells that are alive in this generation*/

        neighbour_1count = 0;
        for(neighbour_1i = 0; neighbour_1i < 6; neighbour_1i++){
          coords[0] = aux->coords[0]; coords[1] = aux->coords[1]; coords[2] = aux->coords[2]; /*re-initialize coords vector to current coords*/
          get_relative_coords(neighbour_1i, world->world_size, coords);

          if((aux1 = ht_find_entry(world->hashtable, world->table_size, coords)) != NULL && aux1->generation != generation && (aux1->next_state == alive || (aux1->next_state == dead && aux1->visited == generation)))
            neighbour_1count++;
          else if(aux1 == NULL || (aux1->next_state == dead && aux1->generation != generation && aux1->visited != generation)){ /*check if empty neighbour will be alive for the next generation*/
            /*If aux1 != NULL, aux1 is the cell that is going to be tested and overwriten if necessary*/
            neighbour_2count = 0;
            for(neighbour_2i = 0; neighbour_2i < 6; neighbour_2i++){
              coords1[0] = coords[0]; coords1[1] = coords[1]; coords1[2] = coords[2];
              get_relative_coords(neighbour_2i, world->world_size, coords1);

              if((aux2 = ht_find_entry(world->hashtable, world->table_size, coords1)) != NULL && aux2->generation != generation && (aux2->next_state == alive || (aux2->next_state == dead && aux2->visited == generation)))
                neighbour_2count++;
            }

            if(cell_get_next_state(dead, neighbour_2count) == alive){
              if(aux1 == NULL){
                ht_new_entry(world, generation, coords[0], coords[1], coords[2]);
              }else{
                aux1->next_state = alive;
                aux1->generation = generation;
              }
              live_cells++; //count cells each time a new one is created
            }
          }
        }

        aux->next_state = cell_get_next_state(alive, neighbour_1count);
        aux->visited = generation;

        if(aux->next_state == alive)
            live_cells++;

      } else  if(aux->generation != generation){ /*cells that were created in this generation are not to be analized or removed*/
        world->hashtable[i] = ht_remove_entry(world->hashtable[i], aux, prev);
        continue; /*Update prev pointer only if current cell not removed*/
      }/*else next_state*/

      prev = aux;

    }/*inner for*/
  }

  /*Place Open MP barrier*/
  world->live_cells = live_cells;
  return world;
}

/*Insertion sort algorithm using coords x, y and z as weights*/
Cell ** sort_world(World *world){
  Cell *aux, *swap;
  Cell **ordered_list = (Cell**) malloc(sizeof(Cell*)*world->live_cells);
  int i, j = 0, min = 0;

  /*Loop copies all cells into a world->lice_cells sized array which will be ordered*/
  for(i = 0; i < world->table_size; i++)
    for(aux = world->hashtable[i]; aux != NULL; aux = aux->next){
      if(aux->next_state != dead){
        ordered_list[j] = aux;
        j++;
      }
    }

  /*Insertion sort with coords as weights*/
  for(i = 1; i < world->live_cells; i++){

    for(j = i; j < world->live_cells; j++){
      if(ordered_list[j]->coords[0] < ordered_list[min]->coords[0]){
        min = j;
      }else if(ordered_list[j]->coords[0] == ordered_list[min]->coords[0]){
        if(ordered_list[j]->coords[1] < ordered_list[min]->coords[1])
          min = j;
        else if(ordered_list[j]->coords[1] == ordered_list[min]->coords[1] && ordered_list[j]->coords[2] < ordered_list[min]->coords[2])
          min = j;
      }
    }
    if(min == i - 1){
      min = i;/*if true, no new minimum was found in this round*/
      continue;
    }

    /*swap minimum*/
    swap = ordered_list[i - 1];
    ordered_list[i - 1] = ordered_list[min];
    ordered_list[min] = swap;

    min = i;
  }
  return ordered_list;
}

State cell_get_next_state(State current_state, int neighbours){
  if( current_state == alive && neighbours >= 2 && neighbours <= 4){
    return alive;
  }
  else if( current_state == dead && (neighbours == 2 || neighbours == 3) ){
    return alive;
  }
  else return dead;
}

void get_relative_coords(int pos_index, int world_size, int *coords){

  switch(pos_index){

      case 0: /*Front*/
        coords[0]++;
        if(coords[0] >= world_size) /*always keep coordinates within range*/
          coords[0] -= world_size;
        break;

      case 1: /*Back*/
        coords[0]--;
        if(coords[0] < 0)
          coords[0] += world_size;
        break;

      case 2:
         coords[1]++;
         if(coords[1] >= world_size)
           coords[1] -= world_size;
        break;

      case 3:
         coords[1]--;
         if(coords[1] < 0)
           coords[1] += world_size;
        break;

      case 4:
         coords[2]++;
         if(coords[2] >= world_size)
           coords[2] -= world_size;
        break;

      case 5:
      default:
         coords[2]--;
         if(coords[2] < 0)
           coords[2] += world_size;
        break;

    }
}
