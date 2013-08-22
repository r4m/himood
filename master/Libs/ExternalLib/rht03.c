//RHT03.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "taskFlyport.h"
#include "sensor.h"
#include "new.h"

struct Interface *attachSensorToDigioBus(void *,int,int);



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


struct RHT03
{
	const void *class;
	struct Interface *inter;
};


static void *RHT03_ctor (void * _self, va_list *app)
{
	struct RHT03 *self = _self;
	return self;
}	

static void RHT03_dtor (void * _sensor)
{
	struct RHT03 *sensor = _sensor;
	free(sensor->inter);
}	

static void RHT03_attach (void * _board,void *_sensor,int n)
{
	struct RHT03 *sensor = _sensor;
	sensor->inter = attachSensorToDigioBus(_board,n,3);
}	


static float RHT03_getdio (void * _self)
{
	struct RHT03 *self = _self;
	IOPut(self->inter->port->Pin1,OFF);
	//DelayUs(10);
	IOPut(self->inter->port->Pin1,ON);
	RPINR7bits.IC1R = 0;
	
	/*
	int ret = IOGet(self->inter->port->Pin1);
	return (float) ret;
*/	return 0;
}	

static const struct SensorClass _RHT03 =
{	
	sizeof(struct RHT03),
	RHT03_ctor,
	RHT03_dtor,
	RHT03_attach,
	0,
	0,
	0,
	RHT03_getdio,
};

const void *Rht03 = &_RHT03;



