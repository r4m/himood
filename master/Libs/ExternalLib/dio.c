//Dio.c
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
	//struct Sensor *sens;
	const void *class;
	int interfaceType;//interface type
	int portNumber; //port number
	GrovePort *port; //porta grove
	struct Interface *next;
	int speed;
	int devAddress[10];
	int devicesNumber;
};


struct Dig_io
{
	const void *class;
	struct Interface *inter;
	BYTE type;
	BYTE pwm_num;
	float* pwm_frequency;
	float* pwm_duty;
	
};


static void *Dig_io_ctor (void * _self, va_list *app)
{
	struct Dig_io *self = _self;
	self->type = va_arg(*app, const BYTE);
	if(self->type == 2)
	{	
		self->pwm_num = va_arg(*app, const BYTE);
		self->pwm_frequency = (float *)malloc(sizeof(float));
		*self->pwm_frequency = 1000;
		self->pwm_duty = (float *)malloc(sizeof(float));
		*self->pwm_duty = 50;
	}
	else
		self->pwm_num = 0;
	return self;
}	

static void Dig_io_dtor (void * _sensor)
{
	struct Dig_io *sensor = _sensor;
	//free(sensor->name);
	free(sensor->inter);
}	


static int Dig_io_setdio (void * _self,int state)
{
	struct Dig_io *self = _self;
	if(self->type  != 2)		
		IOPut(self->inter->port->Pin1,state);
	else
	{
		if(state == ON)
		{
			PWMOn(self->inter->port->Pin1, self->pwm_num);
		}
		else
			PWMOff(self->pwm_num);
		}
	return 0;
}	


static float Dig_io_getdio (void * _self)
{
	struct Dig_io *self = _self;
	int ret = IOGet(self->inter->port->Pin1);
	return (float) ret;
}	


static void Dig_io_attach (void * _board,void *_sensor,int n)
{
	struct Dig_io *sensor = _sensor;
	if(sensor->type == 2)
		PWMInit(sensor->pwm_num, 1000, 50);
	sensor->inter = attachSensorToDigioBus(_board,n,sensor->type);	
}	

static int Dig_io_setPWM(void * _sensor,int type,float data)
{
	struct Dig_io *sensor = _sensor;
	if(!type)
	{
		PWMInit(sensor->pwm_num,data,*sensor->pwm_duty);
		*sensor->pwm_frequency = data;

		}	
	else
		PWMDuty(data,sensor->pwm_num);	
	return 0;
}
static const struct SensorClass _Dig_io =
{	
	sizeof(struct Dig_io),
	Dig_io_ctor,
	Dig_io_dtor,
	Dig_io_attach,
	0,
	0,
	Dig_io_setdio,
	Dig_io_getdio,
	Dig_io_setPWM,	
};

const void *Dig_io = &_Dig_io;


