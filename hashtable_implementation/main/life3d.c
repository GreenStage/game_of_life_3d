#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef DEFS
	#include "../utils/defs.h"
#endif
#include "../hashtable/ht.h"
#include "../utils/error.h"
#include "../world/generations.h"

int main(int argc, char * argv[]){

  int generations, cube_size;
  int x,y,z;
  int i, table_size;
  Cell *aux;
  char line[MAX_LINE_SIZE];
  FILE * inputFile;
  World *world;

  if(argc < 3){
    error_exit("Error: Missing argument",ERR_MISSING_ARG);
  }

  if( strcmp( ".in" , ( argv[1] + strlen(argv[1]) - 3 ) ) != 0 ){
    error_exit("Error: Invalid input file format",ERR_INVALID_INPUT);
  }

  if( ( generations = atoi(argv[2]) ) == 0){
    error_exit("Error: Invalid generations number",ERR_INVALID_GEN);
  }

  inputFile = fopen(argv[1], "r");
  if(inputFile == NULL){
    error_exit("Error: Cant open %s for reading",ERR_OPEN_FILE);
  }

  fgets(line,MAX_LINE_SIZE,inputFile);
  if (!sscanf(line,"%d", &cube_size)){
    error_exit("Error: Invalid cube size",ERR_INVALID_SIZE);
  }

  table_size = cube_size*cube_size*5;

  world = (World *) ht_initialize(table_size, cube_size);

  int count = 0;
  for(i = 0; fgets(line,MAX_LINE_SIZE,inputFile); i++){
    if (!sscanf(line,"%d %d %d", &x,&y,&z))
      error_exit("Error: Invalid position",ERR_INVALID_POS);

    if(ht_new_entry(world, -1, x, y, z))
      count++;
  }

  for(i = 0; i < generations; i++){
    world = next_generations(world, i);
    printf("Generation %d\n", i);
  }


  int total_count = 0;

  for(i = 0; i < world->table_size; i++)
    if(world->hashtable[i] != NULL)
      for(aux = world->hashtable[i]; aux != NULL; aux = aux->next){
        printf("%d %d %d next_state %d\n", aux->coords[0], aux->coords[1], aux->coords[2], aux->next_state);
        if(aux->next_state != dead)
          total_count++;
      }
      printf("Stats: \n");
      printf("count: %d\n", total_count);
      printf("table size %d\n", table_size);
      printf("%d collisions detected\n", count);

  ht_clear(world);
  return 0;
}
