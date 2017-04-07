#include "ht.h"

int hash_code(int *coords, int size){
  return abs((coords[0] + 92821*coords[1] + 3*coords[2]))%size;
}

World * ht_initialize(int size, int world_size){
  int i;

  World *new = (World *) malloc(sizeof(World));
  new->world_size = world_size;
  new->table_size = size;
  new->live_cells = 0;

  new->hashtable_locks = (omp_nest_lock_t *) malloc(size*sizeof(omp_nest_lock_t));
  for(i = 0; i < size; i++)
    omp_init_nest_lock(&(new->hashtable_locks[i])); /*init locks*/

  new->hashtable = (Cell **) malloc(size*sizeof(Cell *));
  memset(new->hashtable, 0, size*sizeof(Cell *));

  return new;
}

int ht_new_entry(World *ht, int generation, int x, int y, int z){
  int hash, coll = 0;

  Cell *new_entry = (Cell *) malloc(sizeof(Cell));
  new_entry->coords = (int *) malloc(3*sizeof(int));
  new_entry->coords[0] = x;
  new_entry->coords[1] = y;
  new_entry->coords[2] = z;
  new_entry->next_state = alive;
  new_entry->generation = generation;
  new_entry->visited = -1;
  new_entry->next = NULL;

  hash = hash_code(new_entry->coords, ht->table_size);
  if(ht->hashtable[hash] != NULL){
    new_entry->next = ht->hashtable[hash];
    coll = 1;
  }
  ht->hashtable[hash] = new_entry;
  return coll;
}

Cell* ht_find_entry(Cell **hashtable, int table_size, int *coords,  omp_nest_lock_t *locks){
  Cell *aux;
  int hash = hash_code(coords, table_size);

/*  while(1){

    if((locked = omp_test_lock(&(locks[hash])))){

      if(is_first){
        if(hashtable[hash] == NULL){
          omp_unset_lock(&(locks[hash]));
          return NULL;
        }
        aux = hashtable[hash];
        is_first = 0;
      }

      if(aux->coords[0] == coords[0] && aux->coords[1] == coords[1] && aux->coords[2] == coords[2]){
        omp_unset_lock(&(locks[hash]));
        return aux;
      }

      aux = aux->next;
      if(aux == NULL){
        omp_unset_lock(&(locks[hash]));
        break;
      }
      omp_unset_lock(&(locks[hash]));
    } else
      printf("FIND ENTRY lock in use pos %d\n", hash);
  }/*end while*/

  omp_set_nest_lock(&(locks[hash]));
  for(aux = hashtable[hash]; aux != NULL; aux = aux->next)
    if(aux->coords[0] == coords[0] && aux->coords[1] == coords[1] && aux->coords[2] == coords[2]){
      omp_unset_nest_lock(&(locks[hash]));
      return aux;
    }

  omp_unset_nest_lock(&(locks[hash]));
  return NULL;
}

Cell* ht_remove_entry(Cell *head, Cell *rm, Cell *rm_prev){
  Cell *updated_head = head;

  if(rm_prev == NULL)
    updated_head = rm->next;
  else
    rm_prev->next = rm->next;

  free(rm->coords);
  free(rm);

  return updated_head;
}


void ht_clear(World *world){
  int i;
  Cell *aux, *jump;

  for(i = 0; i < world->table_size; i++)
    omp_destroy_nest_lock(&(world->hashtable_locks[i])); /*destroy locks*/
  free(world->hashtable_locks);

  for(i = 0; i < world->table_size; i++){
    for(aux = world->hashtable[i]; aux != NULL; aux = jump){
      jump = aux->next;
      free(aux->coords);
      free(aux);
    }
  }
  free(world->hashtable);
  free(world);
}
