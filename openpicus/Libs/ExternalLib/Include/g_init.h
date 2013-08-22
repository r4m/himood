#include "HWlib.h"

#ifndef G_INIT_H
#define G_INIT_H



//	GENERAL USAGE DEFINES
//	Port types defines
#define D3VR_TYPE	0
#define D3V_TYPE	1
#define D5VR_TYPE	2
#define D5V_TYPE	3
#define AN_TYPE		4
#define I2C_TYPE	5
#define DEBUG_TYPE	6
#define UART_TYPE	7




//	Port usage defines
#define NOT_USED	0
#define SINGLE_PORT	1
#define	DOUBLE_PORT	2

//	Pin sharing defines
#define NOT_SHARED	0
#define P1_SHARED	1
#define P2_SHARED	2
#define BOTH_SHARED	3

//	Port initialization return codes
#define INIT_CORRECT			0
#define ERR_WRONG_PORTNUM		1
#define	ERR_PORT_ALREADY_USED	2
#define ERR_WRONG_PORT_TYPE		3

//	Type sensor 
#define GYRO					0
#define COMPASS					1
#define	TOUCHSENSOR				2
#define GPS						3


//	Structures declaration:
//	Structure for the standard Grove port, with two pins. 
//	This type of data can be used for any type of Grove port. 
typedef struct 
{
	BYTE Pin1;			// 	 Pin1 number - Association with Flyport pin number (analog channel for analog ports)
	BYTE Pin2;			//	Pin2 number - Association with Flyport pin number (analog channel for analog ports)
	BYTE Use;			//	Port usage - Indicates if it's used as single or double sensor, or not used
	BYTE Type;			//	Port type - Type of the port, as in the upper defines
	BYTE isShared;		//	Pin sharing - Indicates if the pins of the ports are shared with other ports, like in Arduino UNO Grove shield
}GrovePort;


//	Functions prototypes
//BOOL gBoardInit (BYTE);
BYTE gPortInit (BYTE, BYTE, BYTE, BOOL, int, int);
BYTE gGroveSensor(BYTE, BYTE,BOOL,BOOL);




#endif
