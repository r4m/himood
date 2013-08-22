//A_io class
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "taskFlyport.h"
#include "sensor.h"
#include "new.h"
//#include "g_Board.h"

struct Interface *	attachSensorToAnBus (void *,int);


typedef struct 
{
	BYTE Pin1;			// 	 Pin1 number - Association with Flyport pin number (analog channel for analog ports)
	BYTE Pin2;			//	Pin2 number - Association with Flyport pin number (analog channel for analog ports)
	BYTE Use;			//	Port usage - Indicates if it's used as single or double sensor, or not used
	BYTE Type;			//	Port type - Type of the port, as in the upper defines
	BYTE isShared;		//	Pin sharing - Indicates if the pins of the ports are shared with other ports, like in Arduino UNO Grove shield
}GrovePort;

struct Interface
{
	const void *class;
	int interfaceType;//interface type
	int portNumber; //port number
	GrovePort *port; //porta grove
	struct Interface *next;
	int speed;
	int devAddress[10];
	int devicesNumber;
};



struct Ani
{
	const void *class;
	char *name;//Board's name
	struct Interface *inter;
};


static void *Ani_ctor (void * _self, va_list *app)
{
	struct Ani *self = _self;
	const char *text = va_arg(*app, const char *);
	self->name = malloc(strlen(text));
	strncpy(self->name,text,strlen(text));
	return self;
}	

static void Ani_dtor (void * _sensor)
{
	struct Ani *sensor = _sensor;
	free(sensor->name);
	free(sensor->inter);
}	

static void Ani_attach (void * _board,void *_sensor,int portnumb)
{
	struct Ani *sensor = _sensor;
	sensor->inter = attachSensorToAnBus(_board,portnumb);	
}	


static float Ani_getani (void * _self)
{
	struct Ani *self = _self;
	int ret = ADCVal(self->inter->port->Pin1);
	return (float) ret;
}	



static const struct SensorClass _Ani =
{	
	sizeof(struct Ani),
	Ani_ctor,
	Ani_dtor,
	Ani_attach,
	0,
	0,
	0,
	Ani_getani,
};

const void *Ani = &_Ani;


