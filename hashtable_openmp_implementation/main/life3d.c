#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#ifndef DEFS
	#include "../utils/defs.h"
#endif
#include "../hashtable/ht.h"
#include "../utils/error.h"
#include "../world/generations.h"

int main(int argc, char * argv[]){

  int generations, cube_size;
  int x,y,z, *aux_coords = (int *) malloc(sizeof(int)*3);
  int i, table_size, mult_const, count = 0;
	double start_time, finish_time;
  Cell *aux;
	Cell **ordered_list;
  char line[MAX_LINE_SIZE], buf[4];
  FILE * inputFile;
  World *world;

  if(argc < 3){
    error_exit("Error: Missing argument",ERR_MISSING_ARG);
  }

  if( strcmp( ".in" , ( argv[1] + strlen(argv[1]) - 3 ) ) != 0 ){
    error_exit("Error: Invalid input file format",ERR_INVALID_INPUT);
  }

  if( ( generations = atoi(argv[2]) ) < 1){
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

	if( strcmp( "" , ( argv[1] + strlen(argv[1]) - 3 ) ) != 0 )

	/*Optimize memory size with mult_const to improve performance
		in each test. Best value for mult_const was choosen from
		the array {1, 2, 3, 4, 5} based on performance tests.*/
	if(cube_size == 50)
		mult_const = 5;
	else
		mult_const = 2;

  table_size = cube_size*cube_size*mult_const;
	if(table_size < PARALELLIZE)
		table_size += 400;

	world = (World *) ht_initialize(table_size, cube_size);

	start_time = omp_get_wtime(); /*init time count*/

  for(i = 0; fgets(line,MAX_LINE_SIZE,inputFile); i++){
    if (!sscanf(line,"%d %d %d", &x,&y,&z))
      error_exit("Error: Invalid position",ERR_INVALID_POS);

		if(ht_new_entry(world, -1, x, y, z))
      count++;
  }

  for(i = 0; i < generations; i++)
    world = next_generations(world, i);

	finish_time = omp_get_wtime();

	ordered_list = sort_world(world);
	memset(aux_coords, 0, 3*sizeof(int));
	for(i = 0; i < (world->live_cells + 100) && ordered_list[i] != NULL; i++){
			if(i > 0 && ordered_list[i-1]->coords[0] == ordered_list[i]->coords[0])
				if(ordered_list[i-1]->coords[1] == ordered_list[i]->coords[1])
					if(ordered_list[i-1]->coords[2] == ordered_list[i]->coords[2])
						continue;
			printf("%d %d %d\n", ordered_list[i]->coords[0], ordered_list[i]->coords[1], ordered_list[i]->coords[2]);
	}
	free(aux_coords);

#ifdef DEBUG
	FILE *out = fopen("../test_files/out/output.txt", "w+");
	for(i = 0; i < (world->live_cells + 100) && ordered_list[i] != NULL; i++){
		if(i > 0 && ordered_list[i-1]->coords[0] == ordered_list[i]->coords[0])
			if(ordered_list[i-1]->coords[1] == ordered_list[i]->coords[1])
				if(ordered_list[i-1]->coords[2] == ordered_list[i]->coords[2])
					continue;
		fprintf(out, "%d %d %d\n", ordered_list[i]->coords[0], ordered_list[i]->coords[1], ordered_list[i]->coords[2]);
	}
#endif

#ifdef DEBUG
  printf("Stats \n");
  printf("Cell count: %d \n", world->live_cells);
  printf("table size %d\n", table_size);
  printf("%d initial collisions detected\n", count);
	printf("CPU time: %f\n", finish_time - start_time);
#endif

  ht_clear(world);
#ifdef DEBUG
	free(ordered_list);
#endif
  return 0;
}
