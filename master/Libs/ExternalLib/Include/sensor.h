//sensor.h

//Generic sensor
struct Sensor
{
	const void *class;
};


extern const void *I2C;
extern const void *Sensor;
extern const void *Touch;
extern const void *Dig_io;
extern const void *Ani;

extern const void *Temperature;
extern const void *Gps;

extern const void *Rgb;
extern const void *Rht03;

struct Interface *attachSensorToBus(void *,int,int,int);


/*
extern const void *Gyro;
extern const void *Acc;
*/




