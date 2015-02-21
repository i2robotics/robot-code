#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     HTSPB,          sensorI2CCustom9V)
#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
#pragma config(Motor,  motorB,          left,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     DRIVE_SE,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     DRIVE_NE,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     TUBE,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     FEEDER,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_1,     POPPER,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_2,     FORK,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     DRIVE_SW,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C2_2,     DRIVE_NW,      tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S2_C3_1,    GRAB1,                tServoStandard)
#pragma config(Servo,  srvo_S2_C3_2,    GRAB2,                tServoStandard)
#pragma config(Servo,  srvo_S2_C3_3,    SPOUT,                tServoStandard)
#pragma config(Servo,  srvo_S2_C3_4,    ROOF,                 tServoStandard)
#pragma config(Servo,  srvo_S2_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_6,    FLAP,                 tServoStandard)
#pragma config(Servo,  srvo_S2_C4_1,    servo7,               tServoStandard)
#pragma config(Servo,  srvo_S2_C4_2,    servo8,               tServoNone)
#pragma config(Servo,  srvo_S2_C4_3,    servo9,               tServoNone)
#pragma config(Servo,  srvo_S2_C4_4,    servo10,              tServoNone)
#pragma config(Servo,  srvo_S2_C4_5,    servo11,              tServoNone)
#pragma config(Servo,  srvo_S2_C4_6,    servo12,              tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "../drivers/hitechnic-sensormux.h"
#include "../drivers/hitechnic-irseeker-v1.h"
#include "../drivers/hitechnic-gyro.h"
#include "../drivers/lego-touch.h"
#include "../drivers/hitechnic-superpro.h"

#define IR_SEEK_VAL HTIRSreadDir(msensor_S4_1)
#define GYRO_VAL HTIRSreadDir(msensor_S4_2)
#define GYRO_CAL HTIRSreadDir(msensor_S4_2)
#define SIDE_TOUCH_S TSreadState(msensor_S4_4)
#define SIDE_TOUCH_N TSreadState(msensor_S4_3)

#include "../headers/scaleJoy_1.h"
#include "../headers/gyro_1.h"
#include "../headers/nav_5.h"
#include "../headers/helpers_1.h"
#include "../headers/servoValues_1.h"


task displayIR ()
{
	while (true)
	{
			nxtDisplayBigTextLine(3, "%i    %i", SIDE_TOUCH_N, SIDE_TOUCH_S);
			wait1Msec(300);
				//PlayImmediateTone(ULTRA_VAL*100 + 100,300);
	}
}

void square()
{
  drive_t(E, 88, 0);
  ClearTimer(T1);
  time1[T1] = 0;
  while (true) {
    if (SIDE_TOUCH_N == 1) {
      motor[DRIVE_NE] = 0;
      motor[DRIVE_NW] = 0;
    }
    if (SIDE_TOUCH_S == 1) {
      motor[DRIVE_SE] = 0;
      motor[DRIVE_SW] = 0;
    }
    if (SIDE_TOUCH_N == 1 && SIDE_TOUCH_S == 1) {
      halt();
      PlayImmediateTone(1500, 200);
      break;
    }
  }
  wait1Msec(400);
  drive_e(W, 100, 600); // was 750 worked
}

void swerve(int power, unsigned int time = 600)
{
    motor[DRIVE_NE] = power;
    motor[DRIVE_SE] = power;
    motor[DRIVE_NW] = 0;
    motor[DRIVE_SW] = 0;
    wait1Msec(time);

    motor[DRIVE_NE] = 0;
    motor[DRIVE_SE] = 0;
    motor[DRIVE_NW] = power;
    motor[DRIVE_SW] = power;
    wait1Msec(time*2);
}

task display()
{
	while (true){
		nxtDisplayBigTextLine(4, "%i", time1[T1]);
	}
}

task main()
{
	GRAB_OPEN;
  bool setup_done = false;
  bool fork_down = true;
  if (SPATULA_DOWN == 0) {
  	motor[FORK] = -100;
		fork_down = false;
	}
	if (MAX_REACHED == 0) {
  	motor[TUBE] = 100;
	}
  while (MAX_REACHED == 0) {
    if (SPATULA_DOWN != 0 && !fork_down) {
      if (SPATULA_DOWN != 0) {
        motor[FORK] = 0;
        fork_down = true;
      }
    }
  }
  motor[TUBE] = 0;
  while (!fork_down) {
      if (SPATULA_DOWN != 0) {
        motor[FORK] = 0;
        fork_down = true;
      }
    }
  setup_done = true;
	halt();
	wait1Msec(500);
}
