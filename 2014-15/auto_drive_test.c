#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     HTSPB,          sensorI2CCustom9V)
#pragma config(Sensor, S4,     SMUX,           sensorI2CCustom)
#pragma config(Motor,  motorB,          left,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     DRIVE_SE,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     DRIVE_NE,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     TUBE,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     FEEDER,        tmotorTetrix, openLoop)
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
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "../drivers/hitechnic-sensormux.h"
#include "../drivers/hitechnic-irseeker-v1.h"
#include "../drivers/hitechnic-gyro.h"
#include "../drivers/lego-touch.h"

#define IR_SEEK_VAL HTIRSreadDir(msensor_S4_1)
#define GYRO_VAL HTIRSreadDir(msensor_S4_2)
#define GYRO_CAL HTIRSreadDir(msensor_S4_2)
#define SIDE_TOUCH_S TSreadState(msensor_S4_4)
#define SIDE_TOUCH_N TSreadState(msensor_S4_3)

#include "../headers/scaleJoy_1.h"
#include "../headers/gyro_1.h"
#include "../headers/nav_5.h"
#include "../headers/helpers_1.h"


task main()
{


	drive_t(E, 88, 0);
  ClearTimer(T1);
  while (time1[T1] < 10000) {
  	if(SIDE_TOUCH_N == 1) {
  		motor[DRIVE_NE] = 0;
      motor[DRIVE_NW] = 0;
    }
    if (SIDE_TOUCH_S == 1) {
      motor[DRIVE_SE] = 0;
      motor[DRIVE_SW] = 0;
    }
    if (SIDE_TOUCH_N == 1 && SIDE_TOUCH_S == 1) {
    	halt();
    	break;
    }
  }
  wait1Msec(400);


 // while (SIDE_TOUCH == 0 && time1[T1] < 1000) {}
  drive_e(W, 100, 700); // was 600

	halt();
	PlayImmediateTone(200,200);
	wait1Msec(500);
}
