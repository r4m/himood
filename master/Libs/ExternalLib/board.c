//board.c


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "taskFlyport.h"
#include "new.h"
#include "board.h"
#include "g_Board.h"

int i = 0;

	BYTE g_d1[] = { p2, p5, p8, p10, p12, p17};
	BYTE g_d2[] = { p4, p6, p9, p11, p14, p19};	
	BYTE g_dtype[] = { D5VR_TYPE, D5VR_TYPE, D5VR_TYPE, D5VR_TYPE, D5VR_TYPE, D3VR_TYPE};
	BYTE g_dshared[] = { NOT_SHARED, NOT_SHARED, NOT_SHARED, NOT_SHARED, NOT_SHARED, NOT_SHARED};
	//debug ports type on UART1
	BYTE g_URx[] = { p13 };
	BYTE g_UTx[] ={ p15 }; 
	BYTE g_I2CSCL[] = { p1 };
	BYTE g_I2CSDA[] ={ p3 }; 
	//AN ports type
	BYTE g_an[] ={ 1, 2, 3, 4 }; 
	BYTE g_atype[] = {AN_TYPE, AN_TYPE, AN_TYPE, AN_TYPE};
	BYTE g_ashared[] = { NOT_SHARED, NOT_SHARED, NOT_SHARED, NOT_SHARED};


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


struct Board
{
	const void *class;
	int boardType;
	struct Interface *i2cstart;
	struct Interface *i2cnext;
	struct Interface *diostart;
	struct Interface *dionext;
	struct Interface *anistart;
	struct Interface *aninext;
	struct Interface *uartstart;
	struct Interface *uartnext;
};

BOOL GroveStarted = FALSE;


//costruttore
static void *Board_ctor (void * _self, va_list *app)
{

	struct Board *self = _self;
	self->boardType = va_arg(*app, const BYTE);
	self->i2cstart = NULL;
	self->diostart = NULL;
	self->anistart = NULL;
	return self;
	
}
/*
struct Interface *attachSensorToBus(void *board,int n,int add,int type)
{

	struct Board *self = board;
	struct Interface *p;

	switch(type)
	{
		//creo il primo bus I2C	solo la prima volta
		case 0:
		{
			n--;
			switch(n)
			{
				case 0://bus number 1
				{	
					if(!self->i2cstart)
					{
						self->i2cstart = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus I2C
						self->i2cnext = self->i2cstart;
						self->i2cnext->port = NULL;
						I2CInit(LOW_SPEED);//da togliere
					}
	
					p = self->i2cnext;
					if(!p->port)//collego il bus al groveport scelta	
					{	
						i = 0;
						p->port = (GrovePort *) malloc(sizeof (GrovePort));
						p->port->Pin1 = g_I2CSCL[n];
						p->port->Pin2 = g_I2CSDA[n];
						p->port->Use = DOUBLE_PORT;
						int y;
						for (y= 0;y < 10;y++)
							p->devAddress[y] = 0;
						p->devAddress[i] = add;
						//p->next->port = NULL;
						i++;
						p->devicesNumber = i;
					}
					else
					{
						p->devAddress[i] = add;//attacco il sensore con address add
						i++;
						p->devicesNumber = i;
					}
					break;
				}
		}break;
		}
		//Creo la prima Digio Interface
		case 1:
		{
			n--;

			if(self->diostart != NULL)
			{
				self->diostart = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus I2C
				self->dionext = self->diostart;
				self->dionext->port = NULL;
			}
			p = self->dionext;
		
			while(p != NULL)
			{
				p = p->next;
			}
			if(p == NULL)
			{
				p = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus I2C
				p->next = NULL;
				p->port = NULL;
			}	
			if(p->port == NULL)//collego il bus al groveport scelta	
			{	
				i = 0;
				p->port = (GrovePort *) malloc(sizeof (GrovePort));
				p->port->Pin1 = g_d1[n];
				//add si intende per tipo dell'uscita (IN-OUT-PWM).
				IOInit(p->port->Pin1, add);
				//p->port->Pin2 = g_d2[n];
				p->port->Use = SINGLE_PORT;
				i++;
				p->devicesNumber = i;
				
			}
		}break;
		
		//Analog Input interface
		case 2:
		{

			if(self->anistart != NULL)
			{
				self->anistart = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus I2C
				self->aninext = self->anistart;
				self->aninext->port = NULL;
			}
			p = self->aninext;
		
			while(p != NULL)
			{
				p = p->next;
			}
			if(p == NULL)
			{
				p = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus I2C
				p->next = NULL;
				p->port = NULL;
			}	
			if(p->port == NULL)//collego il bus al groveport scelta	
			{	
				i = 0;
				p->port = (GrovePort *) malloc(sizeof (GrovePort));
				n--;
				p->port->Pin1 = g_an[n];
				p->port->Use = SINGLE_PORT;
				i++;
				p->devicesNumber = i;
				
			}
		}break;	

	}
		
	return p;

}	
*/

struct Interface *attachSensorToI2CBus(void *_board,int i2cbus,int add)
{
	struct Board *self = _board;
	struct Interface *p;
	if(!self->i2cstart)
	{
		self->i2cstart = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus I2C
		self->i2cnext = self->i2cstart;
		self->i2cnext->port = NULL;
		I2CInit(LOW_SPEED);//da togliere
	}
	else
	{
		self->i2cnext = self->i2cstart;
		p = self->i2cnext;
		while(p != NULL)
		{
			p = p->next;
		}
		if(p == NULL)
		{
			p = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus I2C
			p->port = NULL;
		}	
	}

	if(p->port == NULL)//collego il bus al groveport scelta	
	{	
		p->port = (GrovePort *) malloc(sizeof (GrovePort));
		p->port->Pin1 = g_I2CSCL[0];
		p->port->Pin2 = g_I2CSDA[0];
		p->port->Use = DOUBLE_PORT;
		p->next = NULL;
	}
	return p;

}
		

struct Interface *attachSensorToDigioBus(void *_board,int n,int type)
{

	struct Board *self = _board;
	struct Interface *p;
	if(self->diostart == NULL)
	{
		self->diostart = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus DIG_IO
		self->dionext = self->diostart;
		self->dionext->port = NULL;
		p = self->dionext;
	}
	else
	{
		self->dionext = self->diostart;

		p = self->dionext;
		while(p != NULL)
		{
			p = p->next;
		}
		if(p == NULL)
		{
			p = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus I2C
			p->port = NULL;
		}	
	}

	if(p->port == NULL)//collego il bus al groveport scelta	
	{	
		p->port = (GrovePort *) malloc(sizeof (GrovePort));
		p->port->Pin1 = g_d1[n];
		if(type != 2 && type !=5 && type !=3)
		{
			IOInit(p->port->Pin1, type);
			p->port->Use = SINGLE_PORT;
		}
		else if(type != 2 && type ==5)
		{
			IOInit(p->port->Pin1, OUT);
			p->port->Pin2 = g_d2[n];
			IOInit(p->port->Pin2, OUT);
			p->port->Use = DOUBLE_PORT;			
		}	
		else if(type == 3)
			IOInit(p->port->Pin1, inup);
		else
			p->port->Use = SINGLE_PORT;
		p->next = NULL;
	}
	return p;
		
}


struct Interface *	attachSensorToAnBus (void *_board,int n)
{
	struct Board *self = _board;
	struct Interface *p;

	if(self->anistart == NULL)
	{
		self->anistart = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus DIG_IO
		self->aninext = self->anistart;
		self->aninext->port = NULL;
		p = self->aninext;
	}
	else
	{
		self->aninext = self->anistart;
		p = self->aninext;
		while(p != NULL)
		{
			p = p->next;
		}
		if(p == NULL)
		{
			p = (struct Interface *)malloc(sizeof(struct Interface));//punta al primo bus I2C
			p->port = NULL;
		}	
	}

	if(p->port == NULL)//collego il bus al groveport scelta	
	{	
		p->port = (GrovePort *) malloc(sizeof (GrovePort));
		p->port->Pin1 = g_an[n];
		p->port->Use = SINGLE_PORT;		
		p->next = NULL;
	}
	return p;
		
}
	
	

static const struct BoardClass _Board =
{	
	sizeof(struct Board),	
	Board_ctor,
	0,
	0,
};

const void *Board = &_Board;




void insert(struct Interface *pointer,int n,int add)
{
	while(pointer->next!=NULL)
	{
		pointer = pointer -> next;
	}
	pointer->next->port->Pin1 = g_I2CSCL[n-1];
	pointer->next->port->Pin2 = g_I2CSDA[n-1];
	pointer->next->port->Use = DOUBLE_PORT;
	pointer->next->devAddress[i] = add;
	i++;
	pointer = pointer->next;
	pointer->next = NULL;


}





