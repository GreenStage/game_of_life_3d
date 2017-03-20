#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cell.h"

State cell_get_next_state(State current_state, int neighbours){
  if( current_state == alive && neighbours >= 2 && neighbours <= 4){
    return alive;
  }
  else if( current_state == dead && (neighbours == 2 || neighbours == 3) ){
    return alive;
  }
  else return dead;
}

void get_neighbours_by_key(int retval[5], int key){

  switch(key){
    case 0: /*Front*/
      retval[0]=2; retval[1]=6; retval[2]=10;
      retval[3]=11; retval[4]=12;
      break;

    case 1:
       retval[0]=3; retval[1]=7; retval[2]=8;
       retval[3]=9; retval[4]=13;
      break;

    case 2:
       retval[0]=1; retval[1]=5; retval[2]=9;
       retval[3]=11; retval[4]=14;
      break;

    case 3:
       retval[0]=0; retval[1]=4; retval[2]=8;
       retval[3]=10; retval[4]=15;
      break;

    case 4:
       retval[0]=0; retval[1]=1; retval[2]=2;
       retval[3]=3; retval[4]=16;
      break;

    case 5:
       retval[0]=4; retval[1]=5; retval[2]=6;
       retval[3]=7; retval[4]=17;
      break;
    default:
      printf("get_neighbours_by_key ERROR\n\n");
      break;

  }

  return;
}
