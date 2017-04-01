#include "generations.h"

World * next_generations(World *world, int generation){
  int i, coords[3] = {0}, coords1[3] = {0}, neighbour_1i, neighbour_2i, neighbour_1count, neighbour_2count;
  Cell *aux, *prev, *jump, *aux1, *aux2;

  for(i = 0; i < world->table_size; i++){

    prev = NULL;
    for(aux = world->hashtable[i]; aux != NULL; aux = jump){

      jump = aux->next;
      if(aux->next_state == alive){

        neighbour_1count = 0;
        for(neighbour_1i = 0; neighbour_1i < 6; neighbour_1i++){
          coords[0] = aux->coords[0]; coords[1] = aux->coords[1]; coords[2] = aux->coords[2];
          get_relative_coords(neighbour_1i, world->world_size, coords);

          if((aux1 = ht_find_entry(world->hashtable, world->table_size, coords)) != NULL && aux1->generation != generation && (aux1->next_state == alive || (aux1->next_state == dead && aux1->visited == generation))){
            neighbour_1count++;
          }else if(aux1 == NULL || (aux1->next_state == dead && aux1->generation != generation && aux1->visited != generation)){ /*check if empty neighbour will be alive for the next generation*/
            /*If aux1 != NULL, aux1 is the cell that is going to be tested and overwriten if necessary*/
            neighbour_2count = 0;
            for(neighbour_2i = 0; neighbour_2i < 6; neighbour_2i++){
              coords1[0] = coords[0]; coords1[1] = coords[1]; coords1[2] = coords[2];
              get_relative_coords(neighbour_2i, world->world_size, coords1);

              if((aux2 = ht_find_entry(world->hashtable, world->table_size, coords1)) != NULL && aux2->generation != generation && (aux2->next_state == alive || (aux2->next_state == dead && aux2->visited == generation)))
                neighbour_2count++;
            }

            if(cell_get_next_state(dead, neighbour_2count) == alive){
              if(aux1 == NULL)
                ht_new_entry(world, generation, coords[0], coords[1], coords[2]);
               else{
                aux1->next_state = alive;
                aux1->generation = generation;
              }
            }

          //  printf("<%d, %d, %d> second_neighbors %d\n", coords[0], coords[1], coords[2], neighbour_2count);
          }
            //printf("\n<%d, %d, %d> next_state: %d generation %d\n", aux1->coords[0], aux1->coords[1], aux1->coords[2], aux1->next_state, aux1->generation);
        }

        //printf("neighbour 1 count %d\n", neighbour_1count);
        aux->next_state = cell_get_next_state(alive, neighbour_1count);
        aux->visited = generation;

      } else { /*if next_state*/
        world->hashtable[i] = ht_remove_entry(world->hashtable[i], aux, prev);
        continue; /*Update prev pointer only if current cell not removed*/
      }/*else next_state*/

      prev = aux;

    }/*inner for*/
  }
  //printf("Return from %d generation calc\n\n", generation);
  return world;
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
        if(coords[0] >= world_size)
          coords[0] -= world_size;
        break;

      case 1:
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
