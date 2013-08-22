#include "taskFlyport.h"
#include "new.h"
#include "define.h"
#include "board.h"
#include "sensor.h"
#include "g_Board.h"
#include "math.h"
#include "moods.h"

extern BYTE status;
int prevstatus = 100;

char randcolor[9];

int colorIs[3]  = {0,50,100};
int directionIs[3]  = {1,1,1};

int counter = 0;
int greenIs = 240;
int redIs = 20;
int phase = 1;
int step = 40;

void FlyportTask()
{
	void *board = new(Board,GROVE1);
	
	void *rgb1 = new(Rgb,2);
	void *rgb2 = new(Rgb,2);
	void *rgb3 = new(Rgb,2);

	attachSensorToBoard(board,rgb1,PORT1);
	attachSensorToBoard(board,rgb2,PORT2);
	attachSensorToBoard(board,rgb3,PORT3);
	
	set(rgb1,OFF);
	set(rgb2,OFF);
	set(rgb3,OFF);
			
    WFConnect(WF_DEFAULT);
	while (WFStatus != CONNECTED);
	UARTWrite(1,"Flyport connected.\r\n");
	
	while(1)
	{
		switch(status) 
		{
			case SLEEPER:
				if(prevstatus!=(int)status)
				{ 
					prevstatus = status;
					UARTWrite(1,"Mood:: Sleeper!\r\n");
					set(rgb1,OFF);
					set(rgb2,OFF);
					set(rgb3,OFF);
				}
				break;
			case SLACKER:
				if(prevstatus!=(int)status)
				{ 
					prevstatus = status;
					UARTWrite(1,"Mood:: Slacker!\r\n");
				}
				slackerLeds(rgb1, rgb2, rgb3);
				vTaskDelay(1);
				break;
			case ROCKER:
				if(prevstatus!=(int)status)
				{ 
					prevstatus = status;
					UARTWrite(1,"Mood:: Rocker!\r\n");
				}
				rockerLeds(rgb1, rgb2, rgb3);
				vTaskDelay(5);
				break;
			case HACKER:
				if(prevstatus!=(int)status)
				{ 
					prevstatus = status;
					UARTWrite(1,"Mood:: Hacker!\r\n");
				}
				hackerLeds(rgb1, rgb2, rgb3);
				vTaskDelay(10);
				break;
		}
	}
}

void hackerLeds(void *rgb1, void *rgb2, void *rgb3)
{
	switch(counter) 
	{
		case 0:
			updateState();
			configure(rgb1,2,redIs,greenIs,90);
			set(rgb1,ON);
			counter++;
		break;
		case 1:
			updateState();
			configure(rgb1,1,redIs,greenIs,90);	
			set(rgb1,ON);
			counter++;
		break;
		case 2:
			updateState();
			configure(rgb2,1,redIs,greenIs,90);	
			set(rgb2,ON);
			counter++;
		break;
		case 3:
			updateState();
			configure(rgb3,1,redIs,greenIs,90);	
			set(rgb3,ON);
			counter++;
		break;
		case 4:
			updateState();
			configure(rgb3,2,redIs,greenIs,90);	
			set(rgb3,ON);
			counter++;
		break;
		case 5:
			updateState();
			configure(rgb2,2,redIs,greenIs,90);	
			set(rgb2,ON);		
			counter = 0;
		break;
	}
}

void rockerLeds(void *rgb1, void *rgb2, void *rgb3)
{
	int i;
	for(i=1;i<=9;i++)
		randcolor[i] = rand() % 255;

	configure(rgb1,1,randcolor[0],randcolor[1],randcolor[2]);	
	configure(rgb1,2,randcolor[0],randcolor[1],randcolor[2]);
	set(rgb1,ON);

	configure(rgb2,1,randcolor[3],randcolor[4],randcolor[5]);	
	configure(rgb2,2,randcolor[3],randcolor[4],randcolor[5]);
	set(rgb2,ON);
	
	configure(rgb3,1,randcolor[6],randcolor[7],randcolor[8]);	
	configure(rgb3,2,randcolor[6],randcolor[7],randcolor[8]);
	set(rgb3,ON);
}

void slackerLeds(void *rgb1, void *rgb2, void *rgb3)
{
   fade2ColorRed(rgb1);
   fade2ColorGreen(rgb2); 
   fade2ColorBlu(rgb3); 
   set(rgb1,ON); 
   set(rgb2,ON); 
   set(rgb3,ON); 
}

void updateState()
{
	switch(phase) 
	{
		case 1:
			greenIs = 240;
			redIs += step;
			if((redIs+step) > 240)
				phase++;
		break;
		case 2:
			greenIs -= step;
			redIs = 240;
			if((greenIs-step) < 20)
				phase++;
		break;
		case 3:
			greenIs = 20;
			redIs -= step;
			if((redIs-step) < 20)
				phase++;
		break;
		case 4:
			greenIs += step;
			redIs = 20;
			if((greenIs+step) > 240)
				phase = 1;
		break;		
	}
}

void fade2ColorRed(void *rgb)
{
    if (colorIs[0] >= 100)
	{
      directionIs[0] = -1;
    } else if (colorIs[0] <= 0)
	{
	  directionIs[0] = 1;
	  //vTaskDelay(100);
	}
	
	colorIs[0] = colorIs[0] + directionIs[0];
	  
	configure(rgb,1,255,colorIs[0],colorIs[0]);	
	configure(rgb,2,255,colorIs[0],colorIs[0]);
}

void fade2ColorGreen(void *rgb)
{
    if (colorIs[1] >= 100)
	{
      directionIs[1] = -1;
    } else if (colorIs[1] <= 0)
	{
	  directionIs[1] = 1;
	  //vTaskDelay(100);
	}
	
	colorIs[1] += directionIs[1];
	  
	configure(rgb,1,colorIs[1],255,colorIs[1]);	
	configure(rgb,2,colorIs[1],255,colorIs[1]);
}

void fade2ColorBlu(void *rgb)
{
    if (colorIs[2] >= 100)
	{
      directionIs[2] = -1;
    } else if (colorIs[2] <= 0)
	{
	  directionIs[2] = 1;
	  //vTaskDelay(100);
	}
	
	colorIs[2] += directionIs[2];
	  
	configure(rgb,1,colorIs[2],colorIs[2],255);	
	configure(rgb,2,colorIs[2],colorIs[2],255);
}
	
	
	

