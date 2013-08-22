//Temperature_sensor.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "taskFlyport.h"
#include "sensor.h"
#include "new.h"
#include "math.h"

struct Interface *	attachSensorToAnBus (void *,int);

struct Temperature
{
	const void *class;
	char *name;//Board's name
	struct Interface *inter;
};

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


static void *Temp_ctor (void * _self, va_list *app)
{
	struct Temperature *self = _self;
	return self;
}	

static void Temp_dtor (void * _sensor)
{
	struct Temperature *sensor = _sensor;
	free(sensor->inter);
}	

static void Temp_attach (void * _board,void *_sensor,int portnumb)
{
	struct Temperature *sensor = _sensor;
	sensor->inter = attachSensorToAnBus(_board,portnumb);	
}	


static float Temper_getani (void * _self)
{
	struct Temperature *self = _self;
	unsigned int ret = ADCVal(self->inter->port->Pin1);
	float resistance=(float)(1023-ret)*10000/ret; 
	float temperature=1/(log(resistance/10000)/3975+1/298.15)-273.15;

	return temperature;
}	
	
static const struct SensorClass _Temperature =
{	
	sizeof(struct Temperature),
	Temp_ctor,
	Temp_dtor,
	Temp_attach,
	0,
	0,
	0,
	Temper_getani,
};

const void *Temperature = &_Temperature;
