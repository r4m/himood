#ifndef G_BOARD_H
	#define G_BOARD_H

#include "HWlib.h"
#include "g_init.h"

/*	
	Board type define - Insert the code of the board you want to use, 
	put 0 to use a custom board (definition must be inserted below)
	List of supported board:
	0 - Custom board: define you pins and ports
	1 - Grove nest v.1.0
*/
#define BOARDTYPE	1
#define GROVE1	0
#define GROVE2	1

//	*****	Defines for the board type 1 (Standard Grove Nest v.1.0)	*****


//	Defines containing the ports of the boards
//#define D3VR_PORTS	1
//#define D3V_PORTS	0
#define AN_PORTS	3
#define I2C_PORTS 	1
#define DEBUG_PORTS	1
#define DIG_PORTS	5
//amount 10 ports


//	Array used to initialize the board in the file g_init.c.
//	In this way all the init information for the board are
//	included in this header file, while all the initializing
//	functions are inside the g_init.c file.
//	*****	endif for BOARDTYPE 1	*****



//Define other Grove Nest Sensor type
//	*****	endif for BOARDTYPE 2	*****

//define others.......

#endif





