#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     ULTRASONIC,     sensorSONAR)
#pragma config(Sensor, S4,     IR_SEEK,        sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorB,          left,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     WRIST,         tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     ARM,           tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     DRIVE_NW,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     DRIVE_NE,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     DRIVE_SE,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     DRIVE_SW,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S2_C1_1,     FLAG,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     LIFT,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    FLAG_ROT,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    BLK_DROP,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    AUTO_BLK,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Tele-Operation Mode Code Template
//
// This file contains a template for simplified creation of an tele-op program for an FTC
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "./scaleJoy.h" //Our own code to scale joystick values.
#include "./nav_2.00.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of tele-op mode, you may want to perform some initialization on your robot
// and the variables within your program.
//
// In most cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{
	// Place code here to sinitialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

	return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the tele-op robot operation. Customize as appropriate for
// your specific robot.
//
// Game controller / joystick information is sent periodically (about every 50 milliseconds) from
// the FMS (Field Management System) to the robot. Most tele-op programs will follow the following
// logic:
//   1. Loop forever repeating the following actions:
//   2. Get the latest game controller / joystick settings that have been received from the PC.
//   3. Perform appropriate actions based on the joystick + buttons settings. This is usually a
//      simple action:
//      *  Joystick values are usually directly translated into power levels for a motor or
//         position of a servo.
//      *  Buttons are usually used to start/stop a motor or cause a servo to move to a specific
//         position.
//   4. Repeat the loop.
//
// Your program needs to continuously loop because you need to continuously respond to changes in
// the game controller settings.
//
// At the end of the tele-op period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
	initializeRobot();
	const char *functions[] = {"Arm","Lift","Wrist","Flag","Wheels","AUTO_BLK","FLAG_ROT"};
	int selected = 0;
	ClearTimer(T1);
	while (true)
	{
		nMotorPIDSpeedCtrl[WRIST] = mtrSpeedReg;
		nMotorPIDSpeedCtrl[ARM] = mtrSpeedReg;
		eraseDisplay();
		nxtDrawLine (0, 22, 100, 22);
		nxtDisplayStringAt(5, 18, "Ext Batt: %.2f", ((float) externalBatteryAvg)/1000);
		nxtDisplayStringAt(5, 8, "NXT Batt: %.2f", ((float) nAvgBatteryLevel)/1000);


		nxtDisplayStringAt(23, 52, "Function:");
		nxtDisplayStringAt(43, 42, "%s",functions[selected]);
		if (nNxtButtonPressed == 3) {
			if (time10[T1]>30) {
				motor[ARM] = 0;
				motor[LIFT] = 0;
				if (selected == 6) {
  				selected = 0;
  			}
  			else {
  				selected++;
  			}
  			ClearTimer(T1);
  		}
		}
		else if (nNxtButtonPressed == 1) {
			switch (selected) {
				case 0:
					motor[ARM] = 50;
					break;
				case 1:
					motor[LIFT] = 100;
					break;
				case 2:
					motor[WRIST] = 50;
					break;
				case 3:
					motor[FLAG] = 100;
					break;
				case 4:
					drive(N,25,0);
					break;
				case 5:
					servo[AUTO_BLK] = ServoValue[AUTO_BLK]+4;
					nxtDisplayStringAt(43, 32, "%d",ServoValue[AUTO_BLK]);
					break;
				case 6:
					servo[FLAG_ROT] = ServoValue[FLAG_ROT]+4;
					nxtDisplayStringAt(43, 32, "%d",ServoValue[FLAG_ROT]);
					break;
			}
		}
		else if (nNxtButtonPressed == 2) {
			switch (selected) {
				case 0:
					motor[ARM] = -50;
					break;
				case 1:
					motor[LIFT] = -100;
					break;
				case 2:
					motor[WRIST] = -50;
					break;
				case 3:
					motor[FLAG] = -100;
					break;
				case 4:
					drive(S,25,0);
					break;
				case 5:
					servo[AUTO_BLK] = ServoValue[AUTO_BLK]-4;
					nxtDisplayStringAt(43, 32, "%d",ServoValue[AUTO_BLK]);
					break;
				case 6:
					servo[FLAG_ROT] = ServoValue[FLAG_ROT]-4;
					nxtDisplayStringAt(43, 32, "%d",ServoValue[FLAG_ROT]);
					break;
			}
		}
		else {
			motor[ARM] = 0;
			motor[LIFT] = 0;
			halt();
			motor[FLAG] = 0;
			motor[WRIST] = 0;
		}
		wait10Msec(2);
	}
}
