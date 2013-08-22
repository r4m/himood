//Dio.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "taskFlyport.h"
#include "new.h"


struct Dio
{
	const void *class;
	char *name;//Board's name
	BYTE type;
};


static void *Dio_ctor (void * _self, va_list *app)
{
	struct Dio *self = _self;

	const char *text = va_arg(*app, const char *);
	
	self->name = malloc(strlen(text));
	strncpy(self->name,text,strlen(text));

	self->type = va_arg(*app, const BYTE);
	
//Inserire i checks di Gabriele
	return self;
}	


static const struct SensorClass _Dio =
{	
	sizeof(struct Dio),
	Dio_ctor,
};

const void *Dio = &_Dio;


