#ifndef DEFS_HEADER
#define DEFS_HEADER


typedef enum position {
  NONE  = 0,
  FRONT = 0b000000000001, /*1*/
  BACK  = 0b000000000100, /*4*/
  RIGHT = 0b000000010000, /*16*/
  LEFT  = 0b000001000000, /*64*/
  UP    = 0b000100000000, /*256*/
  DOWN  = 0b010000000000, /*1024*/

  F2    = 0b000000000011, /*3*/
  B2    = 0b000000001100, /*12*/

  R2    = 0b000000110000, /*48*/
  L2    = 0b000011000000, /*192*/

  U2    = 0b001100000000, /*768*/
  D2    = 0b110000000000, /*3072*/

  U1_L1 = UP    | LEFT,
  U1_R1 = UP    | RIGHT,
  U1_F1 = UP    | FRONT, /*257*/
  U1_B1 = UP    | BACK,
  D1_L1 = DOWN  | LEFT,
  D1_R1 = DOWN  | RIGHT,
  D1_F1 = DOWN  | FRONT,
  D1_B1 = DOWN  | BACK,
  B1_L1 = BACK  | LEFT,
  B1_R1 = BACK  | RIGHT,
  F1_L1 = FRONT | LEFT,
  F1_R1 = FRONT | RIGHT
}Position;

#define MAX_LINE_SIZE 200

/* define Windows */
#endif
