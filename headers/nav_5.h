/*--------------------------------------------------
 *
 *    Navigation v. 5
 *      utility for Mecanum wheel operation
 *      during Autonomous.
 *    v. 5: to add intelegent feedback control
 *
 *------------------------------------------------*/
#ifdef USING_CLION
#include "../headers/clion_1.h"
#endif

/*#define N 0             //These macros make it easy for the programmer to see
#define S 180             // which direction they are making the robot go,
#define E 90              // instead of needing to figure out an angle
#define W 270
#define NE 45
#define SE 135
#define SW 225
#define NW 315*/
#define CW 401              //Allows for spin functionality
#define AC 402
#define CCW 402              //...And a lot of duplicates for counter-clockwise
#define ACW 402
#define SPIN_RIGHT 401
#define SPIN_LEFT 402

#define N 403
#define S 404
#define E 405
#define W 406
#define NE 407
#define SE 408
#define SW 409
#define NW 410

#define FWD 562              //These macros enable the turn-like-a-car (TLAC)
#define BWD 1074            // functionality. They each set a bit beyond the range
#define RIGHT 2098          // of other uses of the direction imput, to signify
#define LEFT 4146            // the desired direction
// They are (powers of 2) + 50, so the user can input
// this macro + or - 50, and the resulting value, minus
// the bit, will be a usable value from 0 to 100
#define zero 0.01

void halt()
{              //Convinience function to stop all motors.
  motor[DRIVE_NE] = 0;
  motor[DRIVE_SE] = 0;
  motor[DRIVE_NW] = 0;
  motor[DRIVE_SW] = 0;
}

void drive(int d, byte s, unsigned int t, bool useGyro, bool useEnc)
{
  float ne = 0;              //Values that will eventually become motor values.
  float se = 0;
  float nw = 0;
  float sw = 0;

  float x = 0;              //X and Y for Trig functions
  float y = 0;

  int v;                //This is needed for TLAC functionality

  if (d >= 500) {            // TLAC
    if ((512 & d) == 512) {
      v = 2 * ((512 ^ d) - 50);
      ne = s;
      se = s;
      nw = s;
      sw = s;
      if (v < 0) {
        nw -= v;
        sw -= v;
      } else if (v > 0) {
        ne -= v;
        se -= v;
      }
    }  //FWD
    else if ((1024 & d) == 1024) {
      v = 2 * ((1024 ^ d) - 50);
      ne = -s;
      se = -s;
      nw = -s;
      sw = -s;
      if (v < 0) {
        nw = +v;
        sw += v;
      } else if (v > 0) {
        ne += v;
        se += v;
      }
    }  //BWD
    else if ((2048 & d) == 2048) {
      v = 2 * ((2048 ^ d) - 50);
      ne = -s;
      se = s;
      nw = s;
      sw = -s;
      if (v < 0) {
        nw -= v;
        sw += v;
      } else if (v > 0) {
        ne += v;
        se -= v;
      }
    }  //Right
    else if ((4096 & d) == 4096) {
      v = 2 * ((4096 ^ d) - 50);
      ne = s;
      se = -s;
      nw = -s;
      sw = s;
      if (v < 0) {
        nw += v;
        sw -= v;
      } else if (v > 0) {
        ne -= v;
        se += v;
      }
    }  //Left
    else {
      PlayImmediateTone(200, 200);
      wait1Msec(100);
    }
  }
  else {
    switch (d) {
      case CW: //Clockwise
        ne = -1;
        se = -1;
        nw = 1;
        sw = 1;
        useGyro = false;
        break;
      case ACW: //Counter-Clockwise / AntiClockwise
        ne = 1;
        se = 1;
        nw = -1;
        sw = -1;
        useGyro = false;
        break;
        //
        //Old Stuffs: The trig functions below inexplicably stopped working, so I had to
        // reinstate this old code for the main cardinal directions.
        //
      case N: //North / Forwards
        ne = 1;
        se = 1;
        nw = 1;
        sw = 1;
        break;
      case S: //South / Backwards
        ne = -1;
        se = -1;
        nw = -1;
        sw = -1;
        break;
      case E: //East / Right
        ne = -1;
        se = 1;
        nw = 1;
        sw = -1;
        break;
      case W: //West / Left
        ne = 1;
        se = -1;
        nw = -1;
        sw = 1;
        break;
      case NE: //ne
        ne = -zero;
        se = 1;
        nw = 1;
        sw = -zero;
        break;
      case SE: //se
        ne = -1;
        se = zero;
        nw = zero;
        sw = -1;
        break;
      case SW: //sw
        ne = zero;
        se = -1;
        nw = -1;
        sw = zero;
        break;
      case NW: //nw
        ne = 1;
        se = zero;
        nw = zero;
        sw = 1;
        break;
        //
        //end Old Stuffs
        //
      default:
        x = cos((-d + 90.0) * 0.017453278);      //Trigonometry to travel at specified angle.
        y = sin((-d + 90.0) * 0.017453278);      //0.017453278 = pi/180
        writeDebugStreamLine("x: %f, y: %f", x, y);

        ne = (y - x);
        se = (y + x);
        nw = (y + x);
        sw = (y - x);
        break;
    }

    ne = ne * s;
    se = se * s;
    nw = nw * s;
    sw = sw * s;
  }

#ifdef GYRO_INCLUDED
  float targetBearing = bearing;
#endif

  motor[DRIVE_NE] = ne;        //Finally set the motor values
  motor[DRIVE_SE] = se;
  motor[DRIVE_NW] = nw;
  motor[DRIVE_SW] = sw;

   //writeDebugStreamLine(":%i,%i,%i,%i, speed: %i <:NavLn245", ne, se, nw, sw, s);

  if (useEnc) {
    nMotorEncoder[DRIVE_SW] = 0;
    float k_p = 0.08;
    float error;
    while (abs(nMotorEncoder[DRIVE_SW]) < abs(t)) {
    	error = 0;
      //error = k_p * (s / 100.0) * (bearing - targetBearing);
      motor[DRIVE_NE] = ne - error;
      motor[DRIVE_SE] = se - error;
      motor[DRIVE_NW] = nw + error;
      motor[DRIVE_SW] = sw + error;
#ifdef DEBUG_GYRO
      writeDebugStreamLine("err: %f, bear: %f, targ:%f <:NavLn254", error, bearing, targetBearing);
#endif

      wait1Msec(2);
    }
    halt();
  } else if (useGyro && t != 0) {
    time1[T2] = 0;
    float k_p = 0.08;
    float error;
    while (time1[T2] < t) {
    	error = 0;
//      error = k_p * (s / 100.0) * (bearing - targetBearing);
      motor[DRIVE_NE] = ne - error;
      motor[DRIVE_SE] = se - error;
      motor[DRIVE_NW] = nw + error;
      motor[DRIVE_SW] = sw + error;
#ifdef DEBUG_GYRO
      //  writeDebugStreamLine("err: %f, bear: %f, targ:%f <:NavLn268", error, bearing, targetBearing);
#endif
    }
    halt();
  } else if (t != 0) {
    wait1Msec(t);
    halt();
  }
}

void drive_t(int d, byte s = 100, unsigned int t = 0, bool g = false)
{
	drive(d, s, t, g, false);
}
void drive_e(int d, byte s = 100, unsigned int t = 0, bool g = false)
{
  drive(d, s, t, g, true);
}
