#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     HTSPB,          sensorI2CCustom9V)
#pragma config(Sensor, S4,     SMUX,           sensorI2CCustom)
#pragma config(Motor,  motorB,          RIGHT_LIGHT,   tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          LEFT_LIGHT,    tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     DRIVE_SE,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     DRIVE_NE,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     TUBE,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     FEEDER,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_1,     POPPER,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_2,     FORK,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     DRIVE_SW,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S2_C2_2,     DRIVE_NW,      tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S2_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_3,    SPOUT,                tServoStandard)
#pragma config(Servo,  srvo_S2_C3_4,    ROOF,                 tServoStandard)
#pragma config(Servo,  srvo_S2_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_6,    FLAP,                 tServoStandard)
#pragma config(Servo,  srvo_S2_C4_1,    GRAB1,                tServoStandard)
#pragma config(Servo,  srvo_S2_C4_2,    GRAB2,                tServoStandard)
#pragma config(Servo,  srvo_S2_C4_3,    servo9,               tServoNone)
#pragma config(Servo,  srvo_S2_C4_4,    servo10,              tServoNone)
#pragma config(Servo,  srvo_S2_C4_5,    servo11,              tServoNone)
#pragma config(Servo,  srvo_S2_C4_6,    servo12,              tServoNone)

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../headers/helpers_1.h"
#include "../headers/servoValues_1.h"

task main()
{
  GRAB_OPEN;
	while (true)
	{
		if (nNxtButtonPressed == kNxtBtnRight) {
			motor[FORK] = 100;
		}
		else if (nNxtButtonPressed == kNxtBtnLeft) {
			motor[FORK] = -100;
		}
		else{
		motor[FORK]=0;
		}
	}
}
