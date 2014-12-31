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

#define GYRO_INCLUDED

float bearing = 0;
bool do_reset = false;

task updateBearing()
{
  float current_rate = 0;
  float delta_T = 0;
  float prev_bear = 0;

  //GYRO_CAL;

  while (true) {
    time1[T3] = 0;
    current_rate = GYRO_VAL;
    writeDebugStreamLine("rate: %f <:gyroLn35", current_rate);
    nxtDisplayTextLine(1, "rate: %i", current_rate);
    nxtDisplayTextLine(5, "pos: %i", bearing);
    prev_bear = bearing;
    bearing = prev_bear + current_rate * delta_T;

    if (bearing > 360) bearing -= 360;
    else if (bearing < 0) bearing += 360;

    wait1Msec(50);
    delta_T = ((float) time1[T3]) / 1000;
  }
}

void resetBearing()
{
  do_reset = true;
}
