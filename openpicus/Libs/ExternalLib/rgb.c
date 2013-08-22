//RGB_chain.c

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


struct RGB
{
	const void *class;
	struct Interface *inter;
	BYTE led_number;
	BYTE *leds_start;
	BYTE *rgb_matrix;
};

static void soft_clock(struct RGB *self)
{
		vTaskSuspendAll();
		IOPut(self->inter->port->Pin1,ON);
		//Delay10us(1);
		IOPut(self->inter->port->Pin1,OFF);
		//Delay10us(1);
		xTaskResumeAll();
}

static void program(struct RGB *self,BYTE d)
{
	int i = 0;
	BYTE r;
	for(;  i < 8 ;i++)
	{ 
		r = d;
		r = r >> 7;
		IOPut(self->inter->port->Pin2,r);		
		//Delay10us(1);
		soft_clock(self);
		d = d <<1;
	}
	
}

static void default_config(struct RGB *self)
{	
	int j,k;
	self->rgb_matrix = self->leds_start;	
	for(j = 0 ; j < self->led_number ; j++) 
	{
		for(k = 0 ; k < 3 ; k++)
		{
			*self->rgb_matrix = 0;
			self->rgb_matrix++;
		}
	}
	self->rgb_matrix = self->leds_start;		
}


static void *RGB_ctor (void * _self, va_list *app)
{
	struct RGB *self = _self;
	self->led_number = va_arg(*app, const BYTE);
	self->rgb_matrix =(BYTE *)calloc (3* self->led_number, sizeof (BYTE)); 
	self->leds_start = self->rgb_matrix;	
	default_config(self);
	return self;

}	

static void RGB_dtor (void * _sensor)
{
	struct RGB *sensor = _sensor;
	free(sensor->inter);
}	


static int RGB_set (void * _self,int state)
{
	struct RGB *self = _self;
	int y;
	BYTE data  = 0;
	BYTE d  = 0;
	if(state == OFF)
		default_config(self);
	for(y = 0;y <4;y++)
	{
		program(self,data);
	}	

	for(y = 0;y < self->led_number;y++)
	{

		data = data | 0xC0;
		d = (~(*self->rgb_matrix) & (0xC0));
		d = d>>2;
		data = data | d;
		self->rgb_matrix++;
		
		d = (~(*self->rgb_matrix) & (0xC0));
		d = d>>4;
		data = data | d;
		self->rgb_matrix++;
		
		d = (~(*self->rgb_matrix) & (0xC0));
		d = d>>6;
		data = data | d;

		program(self,data);

		/*2° BYTE =  blue*/
		self->rgb_matrix-=2;
		program(self,*self->rgb_matrix);
		self->rgb_matrix++;

		/*3° BYTE =  green*/
		program(self,*self->rgb_matrix);
		self->rgb_matrix++;

		/*4° BYTE =  red*/
		program(self,*self->rgb_matrix);
		self->rgb_matrix++;

		}

		data = 0;
		for(y = 0;y <4;y++)
		{
			program(self,data);
		}	
	
	return 0;
}	


static void RGB_attach (void * _board,void *_sensor,int n)
{
	struct RGB *sensor = _sensor;
	//5 means RGB type object
	sensor->inter = attachSensorToDigioBus(_board,n,5);	
	const struct SensorClass **cp = _sensor;
	if(_sensor && *cp && (*cp)->set_)
		(*cp)->set_(_sensor,ON);
}	

static void RGB_config (void * _self, va_list *app)
{
	struct RGB *self = _self;
	self->rgb_matrix = self->leds_start;
	BYTE numb = va_arg(*app, const BYTE);
	BYTE red = va_arg(*app, const BYTE);
	BYTE green = va_arg(*app, const BYTE);
	BYTE blue = va_arg(*app, const BYTE);
	
	self->rgb_matrix = self->rgb_matrix + ( 3*(numb-1));
	*self->rgb_matrix = blue;
	self->rgb_matrix++;
	*self->rgb_matrix = green;
	self->rgb_matrix++;
	*self->rgb_matrix = red;
	self->rgb_matrix = self->leds_start;

}

static const struct SensorClass _RGB =
{	
	sizeof(struct RGB),
	RGB_ctor,
	RGB_dtor,
	RGB_attach,
	RGB_config,
	0,
	RGB_set,
	0,
	0,	
};

const void *Rgb = &_RGB;


