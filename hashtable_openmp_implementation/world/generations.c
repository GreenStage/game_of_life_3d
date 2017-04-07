#include "generations.h"

World * next_generations(World *world, int generation){
  int i, coords[3] = {0}, coords1[3] = {0}, neighbour_1i, neighbour_2i, neighbour_1count, neighbour_2count;
  int live_cells = 0;
  Cell *aux, *prev, *jump, *aux1, *aux2;

int chunk = (int) (world->table_size/500);
//omp_set_schedule(omp_sched_static, (int) (world->table_size/200));

#pragma omp parallel if(world->table_size > PARALELLIZE) shared(world) private(aux, prev, jump, aux1, aux2, neighbour_1i, neighbour_2i, neighbour_1count, neighbour_2count) reduction(+:live_cells)
{


#pragma omp for firstprivate(coords, coords1)  schedule(static, chunk)
  for(i = 0; i < world->table_size; i++){
#pragma omp flush(world)
    prev = NULL;
    for(aux = world->hashtable[i]; aux != NULL; aux = jump){

      jump = aux->next;
      if(aux->next_state == alive && aux->generation != generation){ /*Only analize cells that are alive in this generation*/

        neighbour_1count = 0;
        for(neighbour_1i = 0; neighbour_1i < 6; neighbour_1i++){
          coords[0] = aux->coords[0]; coords[1] = aux->coords[1]; coords[2] = aux->coords[2]; /*re-initialize coords vector to current coords*/
          get_relative_coords(neighbour_1i, world->world_size, coords);
          if((aux1 = ht_find_entry(world->hashtable, world->table_size, coords, world->hashtable_locks)) != NULL && aux1->generation != generation && (aux1->next_state == alive || (aux1->next_state == dead && aux1->visited == generation))){
            neighbour_1count++;

          } else if(aux1 == NULL || (aux1->next_state == dead && aux1->generation != generation && aux1->visited != generation)){ /*check if empty neighbour will be alive for the next generation*/
            /*If aux1 != NULL, aux1 is the cell that is going to be tested and overwriten if necessary*/
            neighbour_2count = 0;
            for(neighbour_2i = 0; neighbour_2i < 6; neighbour_2i++){
              coords1[0] = coords[0]; coords1[1] = coords[1]; coords1[2] = coords[2];
              get_relative_coords(neighbour_2i, world->world_size, coords1);
              if((aux2 = ht_find_entry(world->hashtable, world->table_size, coords1, world->hashtable_locks)) != NULL && aux2->generation != generation && (aux2->next_state == alive || (aux2->next_state == dead && aux2->visited == generation)))
                neighbour_2count++;
            }

            if(cell_get_next_state(dead, neighbour_2count) == alive){
             omp_set_nest_lock(&(world->hashtable_locks[hash_code(coords, world->table_size)]));
              if(aux1 == NULL){
                ht_new_entry(world, generation, coords[0], coords[1], coords[2]);
                live_cells++;
              } else{
                aux1->next_state = alive;
                aux1->generation = generation;
                live_cells++;
              }
              omp_unset_nest_lock(&(world->hashtable_locks[hash_code(coords, world->table_size)]));
            }
          }
        }

        aux->next_state = cell_get_next_state(alive, neighbour_1count);
        aux->visited = generation;

        if(aux->next_state == alive)
            live_cells++;


      } else  if(aux->generation != generation){ /*cells that were created in this generation are not to be analized or removed*/
        omp_set_nest_lock(&(world->hashtable_locks[i])); /*lock hashtable entry i for update (cell removal)*/
        world->hashtable[i] = ht_remove_entry(world->hashtable[i], aux, prev);
        omp_unset_nest_lock(&(world->hashtable_locks[i]));
        continue; /*Update prev pointer only if current cell not removed*/
      }/*else next_state*/

      prev = aux;
    }/*inner for*/
  }/*outter for*/
} /*end parallel region*/

  world->live_cells = live_cells;
  return world;
}

/*Insertion sort algorithm using coords x, y and z as weights*/
Cell ** sort_world(World *world){
  Cell *aux, *swap;
  Cell **ordered_list = (Cell**) malloc(sizeof(Cell*)*(world->live_cells + 100)); /*+ 100 to prevent creating too short vector due to wrong cell count*/
  int i, j = 0, min = 0;
  printf("%d\n", world->live_cells);

  /*Loop copies all cells into a world->lice_cells sized array which will be ordered*/
  memset(ordered_list, NULL, sizeof(Cell*)*(world->live_cells + 100));
  for(i = 0; i < world->table_size; i++)
    for(aux = world->hashtable[i]; aux != NULL; aux = aux->next){
      if(aux->next_state != dead){
        ordered_list[j] = aux;
        j++;
      }
    }
    printf("Loaded ordererd list count %d\n", j);
  /*Insertion sort with coords as weights*/
  for(i = 1; i < (world->live_cells + 100) && ordered_list[i] != NULL; i++){

    for(j = i; j < (world->live_cells + 100) && ordered_list[j] != NULL; j++){
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
  printf("If it reached here, there should be no errors\n");

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
