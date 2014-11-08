#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S3,     GYRO,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     HTSPB,                sensorI2CCustom9V)
#pragma config(Motor,  motorB,          left,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     DRIVE_SE,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     DRIVE_NE,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     LIFT,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     FEEDER,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_1,     POPPER,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     DRIVE_SW,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     DRIVE_NW,      tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C3_1,    grab1,                tServoStandard)
#pragma config(Servo,  srvo_S2_C3_2,    grab2,                tServoStandard)
#pragma config(Servo,  srvo_S2_C3_3,    flip,                 tServoStandard)
#pragma config(Servo,  srvo_S2_C3_4,    roof,                 tServoStandard)
#pragma config(Servo,  srvo_S2_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "../headers/scaleJoy_1.h"
#include "../headers/helpers_1.h"
#include "../drivers/HTSPB-driver.h"
//#include "../headers/liftControl_1.h"
float J1X1;
float J1Y1;
float J1X2;

float J2Y2;
short current_joystick;
task main()
{
	//nMotorEncoder[DRIVE_SE] = 0;
	// nMotorEncoder[DRIVE_SW] = 0;
	//StartTask(updateBearing);
	while (true) {
		getJoystickSettings(joystick);
		J1X1 = scaleJoy(joystick.joy1_x1);
		J1X2 = scaleJoy(joystick.joy1_x2);
		J1Y1 = scaleJoy(joystick.joy1_y1);


		J2Y2 = scaleJoy(joystick.joy2_y2);

		//nxtDisplayTextLine(2, "SE:%i, SW:%i", nMotorEncoder[DRIVE_SE], nMotorEncoder[DRIVE_SW]);

		//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

		action_joy1
		state bLB down
		motor[FEEDER] = 100;
		state bLT down
		motor[FEEDER] = -100;
		otherwise
		motor[FEEDER] = 0;
		end

		action_joy1
		state bY down //grab
		servo[grab1] = 8;
		servo[grab2] = 239;
		state bA down //release
		servo[grab1] = 232;
		servo[grab2] = 16;
		end

		action_joy2
		state bRT down
		servo[roof] = 180;
		wait1Msec(1000);
		servo[flip] = 0;
		state bRB down
		servo[flip] = 255;
		wait1Msec(1000);
		servo[roof] = 60;
		end

		action_joy2
		state bB down
		motor[POPPER] = 100;
		otherwise
		motor[POPPER] = 0;
		end

		//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
		HTSPBsetupIO(HTSPB, 0x10);
		if(HTSPBreadIO(HTSPB, 0x01) != 1) {
			nxtDisplayTextLine(1, "Magnet absent");
			//HTSPBwriteIO(HTSPB, 0x10);
			} else {
			nxtDisplayTextLine(1, "Magnet present");
			//HTSPBwriteIO(HTSPB, 0x00);
		}
		//StartTask(lift_control);
		if (HTSPBreadIO(HTSPB, 0x01) != 1 || J2Y2<0) {
			motor[LIFT] = J2Y2;
			} else {
			motor[LIFT] = 0;
		}
		//nxtDisplayTextLine(1, "Height: %i", lift_height);
		//action_joy2
		// state bA down
		// 	height = 0;
		// end

		//if (height == 0 && (HTSPBreadIO(HTSPB, 0x01) != 1)) {
		//	motor[LIFT] = 100;
		//} else {
		//	motor[LIFT] = 0;
		//	height = 1;
		//}
		motor[DRIVE_NE] = (J1Y1 - J1X1 - J1X2);
		motor[DRIVE_SE] = (J1Y1 + J1X1 - J1X2);
		motor[DRIVE_NW] = (J1Y1 + J1X1 + J1X2);
		motor[DRIVE_SW] = (J1Y1 - J1X1 + J1X2);
		writeDebugStreamLine("Y1:%g, X1:%g, X2:%g", J1Y1, J1X1, J1X2);
	}
}
