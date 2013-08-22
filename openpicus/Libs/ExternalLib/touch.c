///touch.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "taskFlyport.h"
#include "new.h"
#include "sensor.h"
#include "define.h"
#include "mpr121.h"


struct Interface *attachSensorToI2CBus(void *,int,int);


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


struct Touch
{
	const void *class;
	struct Interface *inter;
	BYTE pads;//pads number installed
	BYTE implement;
	BYTE devaddress; //already shifted;
	BYTE datapre;
};




static BYTE read_register(BYTE address,BYTE regist)
{
    I2CStart();
	I2CWrite(address  | 0);
	#ifdef DEBUG
		if(I2C1STATbits.ACKSTAT)
		{
			UARTWrite(1,"error....\n\r");
		}
		char deb[20];
		//sprintf (deb,"sensor'address 0X%02X\n\r",address );
		UARTWrite(1,deb);
	#endif
	I2CWrite(regist);
	#ifdef DEBUG
		if(I2C1STATbits.ACKSTAT)
		{
			UARTWrite(1,"errore....\n\r");
		}
		sprintf (deb,"regist 0X%02X\n\r",regist);
		UARTWrite(1,deb);
	#endif
	I2CRestart();
	I2CWrite(address  | 1);
	#ifdef DEBUG
		if(I2C1STATbits.ACKSTAT)
		{
			UARTWrite(1,"errore....\n\r");
		}
	#endif
	BYTE data = I2CRead(1);
	I2CStop();
	I2CStop();
	I2CStop();
	return data;

}

static int Touch_read_ (void * _dev)
{

	struct Touch *dev = _dev;
/*
	int pad = 0;
	int state = 0;
	int d,pd,r = 0;
*/

	BYTE data = 0;
	data =	read_register(dev->devaddress, STATUS1);
	data = data & 0x0F;
	if (dev->pads > 4)
		data =data | (read_register(dev->devaddress, STATUS2)  << 4);
	//BYTE data1 = data | ((read_register(dev->devaddress, STATUS2)<<4));
/*
	char msg[45];
	sprintf(msg,"data %u\n\r",data);
	UARTWrite(1,msg);
*/

	
/*	pad--;
	d = (data>>pad) & 1;
	pd = (dev->datapre>>pad)& 1;

	if(state)
	{
	if(d != pd)
	{	

		if(d)
			{
				r = 1;
		dev->datapre = data;	

				}

}			
}
		if(!state)
	{	
	if(d != pd)
		
		{
			if(pd)
			{
				r = 1;
				dev->datapre = data;	
	}
		}
			}
	*/
	if(dev->datapre == data)
		return 0;//same previuos value
	else
		dev->datapre = data;	
	return data;
	
}	

static void set_register(BYTE address,BYTE regist, BYTE data)
{

    I2CStart();
	I2CWrite(address  | 0);
	#ifdef DEBUG
		if(I2C1STATbits.ACKSTAT)
		{
			UARTWrite(1,"error....\n\r");
			while(1);
		}
		char deb[20];
		//sprintf (deb,"address 0X%02X\n\r",(address>>1));
		UARTWrite(1,deb);
	#endif
	I2CWrite(regist);
	#ifdef DEBUG
		if(I2C1STATbits.ACKSTAT)
		{
			UARTWrite(1,"error....\n\r");
			while(1);
		}
		sprintf (deb,"regist 0X%02X\n\r",regist);
		UARTWrite(1,deb);
	#endif
	I2CWrite(data);
	#ifdef DEBUG
		if(I2C1STATbits.ACKSTAT)
		{
			UARTWrite(1,"error....\n\r");
			while(1);
		}
		sprintf (deb,"data 0X%02X\n\r",data);
		UARTWrite(1,deb);
	#endif
	I2CStop();

	TMR4 = 0;
}




static void groveTouchSensorEnable(BYTE address, BYTE padsnumb)
{
	if (padsnumb > 4)
		padsnumb+=4;
	set_register(address, ELE_CFG, padsnumb);
}	





static void init(struct Touch *self)
{

	set_register(self->devaddress,ELE_CFG,0);
	

	// Section A - Controls filtering when data is > baseline.
	    set_register(self->devaddress, MHD_R, 0x01);
	    set_register(self->devaddress, NHD_R, 0x01);
	    set_register(self->devaddress, NCL_R, 0x00);
	    set_register(self->devaddress, FDL_R, 0x00);
	    
	    // Section B - Controls filtering when data is < baseline.
	    set_register(self->devaddress, MHD_F, 0x01);
	    set_register(self->devaddress, NHD_F, 0x01);
	    set_register(self->devaddress, NCL_F, 0xFF);
	    set_register(self->devaddress, FDL_F, 0x02);
	    
	    // Section C - Sets touch and release thresholds for each electrode
	    set_register(self->devaddress, ELE0_T, TOU_THRESH);
	    set_register(self->devaddress, ELE0_R, REL_THRESH);
	    set_register(self->devaddress, ELE1_T, TOU_THRESH);
	    set_register(self->devaddress, ELE1_R, REL_THRESH);

	    set_register(self->devaddress, ELE2_T, TOU_THRESH);
	    set_register(self->devaddress, ELE2_R, REL_THRESH);
	    
	    set_register(self->devaddress, ELE3_T, TOU_THRESH);
	    set_register(self->devaddress, ELE3_R, REL_THRESH);
	    
	    set_register(self->devaddress, ELE4_T, TOU_THRESH);
	    set_register(self->devaddress, ELE4_R, REL_THRESH);
	    
	    set_register(self->devaddress, ELE5_T, TOU_THRESH);
	    set_register(self->devaddress, ELE5_R, REL_THRESH);
	    
	    set_register(self->devaddress, ELE6_T, TOU_THRESH);
	    set_register(self->devaddress, ELE6_R, REL_THRESH);
	    
	    set_register(self->devaddress, ELE7_T, TOU_THRESH);
	    set_register(self->devaddress, ELE7_R, REL_THRESH);
	    
	    set_register(self->devaddress, ELE8_T, TOU_THRESH);
	    set_register(self->devaddress, ELE8_R, REL_THRESH);
	    
	    set_register(self->devaddress, ELE9_T, TOU_THRESH);
	    set_register(self->devaddress, ELE9_R, REL_THRESH);
	    
	    set_register(self->devaddress, ELE10_T, TOU_THRESH);
	    set_register(self->devaddress, ELE10_R, REL_THRESH);
	    
	    set_register(self->devaddress, ELE11_T, TOU_THRESH);
	    set_register(self->devaddress, ELE11_R, REL_THRESH);
	


	 set_register(self->devaddress, 0x77, 0x0F);

	groveTouchSensorEnable(self->devaddress,self->pads);

}


static void *Touch_ctor (void * _self, va_list *app)
{

	struct Touch *self = _self;
	self->pads = va_arg(*app, const BYTE);
	self->implement = va_arg(*app, const BYTE);
	self->devaddress =  va_arg(*app, const BYTE);
	

	self->datapre = 0;
	return self;

}


static void Touch_attach (void * _board,void *_sensor,int ic2bus)
{
	struct Touch *sensor = _sensor;
	int add = sensor->devaddress;
	sensor->inter = attachSensorToI2CBus(_board,ic2bus,add);
}


static void Touch_config (void * _self, va_list *app)
{
	struct Touch *self = _self;
	init(self);	
}


static int Touch_event (void * _self)
{
	struct Touch *self = _self;
	return Touch_read_(self);
	
}



static const struct SensorClass _Touch =
{	
	sizeof(struct Touch),
	Touch_ctor,
	0,
	Touch_attach,
	Touch_config,
	Touch_event,


};

const void *Touch = &_Touch;



//end
