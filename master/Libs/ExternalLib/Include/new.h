//new.h


#include<stdlib.h>
#include <stdarg.h>


void * new(const void *,...);
int attachSensorToBoard(void *,void *,int);
void configure(void *,...);
int TouchEvent(void *);
int set(void *,int);
float get(void *);
int setPWM(void *,int,float);

struct SensorClass
{
	size_t size; 
	void *(* ctor) (void *,va_list *);
	void (* dtor) (void *);
	void (*attach) (void *,void *,int);
	void (*config) (void *,va_list *);
	int (*event) (void *);
	int (*set_)(void *,int);
	float (*getdio)(void *);
	int (*set_PWM)(void *,int,float);	
/*
	char *(*gName) ( void *);
	void  (*init)(void *);
	void  *(*read_)(void *);
*/
};

struct BoardClass
{
	size_t size; 
	void *(* ctor) (void *,va_list *);
	void *(* dtor) (void *);
	int (*attach)  (void *,void *,int,int);
	void (*start) (const void *);
	char *(*gName) (const void *);


};

