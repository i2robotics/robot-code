#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorB,          left,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     DRIVE_NW,      tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     DRIVE_NE,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     DRIVE_SE,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     DRIVE_SW,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     ARM,           tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     b,             tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     a,             tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    ARM_ROT,              tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "./scaleJoy.h"

float X1;
float Y1;
float X2;

task main()
{
	while (true) {
		getJoystickSettings(joystick);
		nMotorPIDSpeedCtrl[DRIVE_NE] = mtrSpeedReg;
		nMotorPIDSpeedCtrl[DRIVE_NW] = mtrSpeedReg;
		nMotorPIDSpeedCtrl[DRIVE_SE] = mtrSpeedReg;
		nMotorPIDSpeedCtrl[DRIVE_SW] = mtrSpeedReg;
		X1 = scaleJoy(joystick.joy1_x1);
		X2 = scaleJoy(joystick.joy1_x2);
		Y1 = scaleJoy(joystick.joy1_y1);


		motor[DRIVE_NE] = Y1-X1-X2;
		motor[DRIVE_SE] = Y1+X1-X2;
		motor[DRIVE_NW] = Y1+X1+X2;
		motor[DRIVE_SW] = Y1-X1+X2;

	}
}
