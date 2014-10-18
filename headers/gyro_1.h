/*--------------------------------------------------
*
*		Gyroscope v. 5
*			utility for using the HiTechnic
*			Gyroscope Sensor.
*
*------------------------------------------------*/

/*  Definitions
*     Heading: The direction in which the robot is moving
*					relative to the front of the robot; the velocity vector.
*			Bearing: The direction the robot is facing, relative
*					to an arbitrary angle.
*/

#include "../drivers/hitechnic-gyro.h"

#define GYRO_INCLUDED indeed

float bearing = 0;

task updateBearing ()
{
	float current_rate = 0;
	float delta_T = 0;
	float prev_bear = 0;

	HTGYROstartCal(GYRO);

	while(true)
	{
		time1[T3] = 0;
		current_rate = HTGYROreadRot(GYRO);
		nxtDisplayBigTextLine(3, "rate: %i", current_rate);
		prev_bear = bearing;
		bearing = prev_bear + current_rate*delta_T;

		if (bearing > 360) bearing -= 360;
		else if (bearing < 0) bearing += 360;

		wait1Msec(50);
		delta_T = ((float)time1[T3]) / 1000;
	}
}
