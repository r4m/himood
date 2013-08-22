//touch.c

//port_.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "taskFlyport.h"
#include "new.h"
#include "sensor.h"


/*
void *Sensor_dtor (void * _self)
{

	struct Sensor *self = _self;
	
	free(self->name);
	return self;
}



static const struct SensorClass _Sensor =
{	
	sizeof(struct Sensor),
	0,
	Sensor_dtor,
	0,
	0,	
};

const void *Sensor = &_Sensor;





static const struct SensorClass _Gps =
{	
	sizeof(struct Gps),
	Gps_ctor,
	Gps_dtor,
	0,
	Gps_config,
};

const void *Gps = &_Gps;

*/
