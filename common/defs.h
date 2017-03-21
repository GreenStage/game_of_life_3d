#ifndef DEFS_HEADER
#define DEFS_HEADER

#define DEBUG

typedef enum bool_ { false = 0, true } bool;
typedef enum mirror_ {
	near_none = 0x0,
	near_x0 = 0x1,
	near_xmax = 0x2,
	near_y0 = 0x4,
	near_ymax = 0x8,
	near_z0 = 0x10,
	near_zmax = 0x20
} mirror;
typedef enum rel_position {
	NONE  = 0,
	FRONT = 0b000000000001, /*1*/
	BACK  = 0b000000000100, /*4*/
	RIGHT = 0b000000010000, /*16*/
	LEFT  = 0b000001000000, /*64*/
	UP    = 0b000100000000, /*256*/
	DOWN  = 0b010000000000, /*1024*/

	F2 = 0b000000000011, /*3*/
	B2 = 0b000000001100, /*12*/
	R2 = 0b000000110000, /*48*/
	L2 = 0b000011000000, /*192*/
	U2 = 0b001100000000, /*768*/
	D2 = 0b110000000000, /*3072*/

	F1_L1 = FRONT | LEFT,
	B1_R1 = BACK | RIGHT,

	F1_R1 = FRONT | RIGHT,
	B1_L1 = BACK | LEFT,

	U1_L1 = UP | LEFT,
	D1_R1 = DOWN | RIGHT,

	U1_R1 = UP | RIGHT,
	D1_L1 = DOWN | LEFT,

	U1_F1 = UP | FRONT, /*257*/
	D1_B1 = DOWN | BACK,

	U1_B1 = UP | BACK,
	D1_F1 = DOWN | FRONT
}relative_position;

#define MAX_LINE_SIZE 200
#define ABSOLUTE(Y) ( (Y)  < (0) ? (-Y) : (Y) )
#define min_abs(A,B) ( (ABSOLUTE(A) < ABSOLUTE(B)) ? (A) : (B) )

/* define Windows */
#endif