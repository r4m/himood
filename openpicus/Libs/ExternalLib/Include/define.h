//new.h


//i2c speed
#define LOW 	0x9DuL
#define HIGH 	0

//UART speed
#define SPEED1 	19200uL


//Implementation
#define POLL 	0
#define INT 	1

//DIO
#define PWM 	2


//Touch sensor address
#define TOUCHADD0 	0xB4

//fittizzi
#define TOUCHADD1 	0xB3
#define TOUCHADD2 	0xB2
#define TOUCHADD3 	0xB1
#define AUTOCONF	39

#define BOTH		2
#define PRESSED_	1
#define RELEASED_	0


#define PADSNUMBER	4
#define IRQ			2
#define TMR			1
#define POLL		0




#define PORT1	  	0
#define PORT2	  	1
#define PORT3	  	2
#define PORT4	  	3
#define PORT5	  	4
#define PORT6	  	0
#define PORT7	  	1
#define PORT8	  	2
#define PORT9	  	0
#define PORT10	  	0

#define PWM		  	2
#define FREQUENCY	0
#define DUTY		1

#define PAD1		1
#define PAD2		2
#define PAD3		4
#define PAD4		8
#define PAD5		16
#define PAD6		32
#define PAD7		64
#define PAD8		128
