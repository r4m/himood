//new.c
 

#include "new.h"
#include <stdlib.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sensor.h"
#include "board.h"
#include <stdio.h>



static void *new_sensor(const void *,va_list ap);
static void *new_board(const void *,va_list ap);


//crea un nuovo Oggetto e apre le comunicazioni
void *new(const void *_class, ...)
{
	//mi permette di utilizzare il costruttore corretto
	va_list ap;
	va_start(ap,_class);

	if (*( size_t *) _class == *(size_t *) Board)
	{
		return new_board(_class,&ap);			
	}
	else
	{
		return new_sensor(_class,&ap);			
		
	}
	

}

//elimina l'oggetto
void delete(void *self)
{
	const struct SensorClass **cp = self;
	
	if(self && *cp && (*cp)->dtor)
		(*cp)->dtor(self);
	free(self);
	

}

int attachSensorToBoard(void *board,void *sensor,int numb)
{

	const struct SensorClass **sen = sensor;
	if(!(sensor && *sen && (*sen)->attach))
		return 1;
	(*sen)->attach(board,sensor,numb);
return 0;
}

void configure(void *self, ...)
{
	va_list ap;
	va_start(ap,self);
	const struct SensorClass **cp = self;
	if(self && *cp && (*cp)->config)
		(*cp)->config(self,&ap);
	va_end(ap);
}

int TouchEvent(void *self)
{
	const struct SensorClass **cp = self;
	if(self && *cp && (*cp)->event)
		return (*cp)->event(self);
	return -1;
}


int set(void *self,int state)
{
	const struct SensorClass **cp = self;
	if(self && *cp && (*cp)->set_)
		return (*cp)->set_(self,state);
	return -1;
}

float get(void *self)
{
	const struct SensorClass **cp = self;
	if(self && *cp && (*cp)->getdio)
		return (*cp)->getdio(self);
	return -1;
}

int setPWM(void *self,int type,float data)
{
	const struct SensorClass **cp = self;
	if(self && *cp && (*cp)->set_PWM)
			return (*cp)->set_PWM(self,type,data);
	return -1;
}


void *new_board(const void *_class,va_list ap)
{
	const struct BoardClass *class = _class;
	void *p = calloc(1, class->size);
	*(const struct BoardClass **)p = class;
	if(class->ctor)
	{
		p = class->ctor(p, ap);
		va_end(ap);
	}

	return p;

}

void *new_sensor(const void *_class,va_list ap)
{
	const struct SensorClass *class = _class;
	void *p = calloc(1, class->size);
	*(const struct SensorClass **)p = class;
	if(class->ctor)
	{
		p = class->ctor(p, ap);
		va_end(ap);

	}

	return p;
}
