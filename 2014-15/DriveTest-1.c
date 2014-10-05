#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     ULTRASONIC,     sensorSONAR)
#pragma config(Sensor, S4,     IR_SEEK,        sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorB,          left,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     DRIVE_NW,      tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     DRIVE_NE,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     DRIVE_SE,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     DRIVE_SW,      tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     motorH,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_1,     motorJ,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     motorK,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
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

float X1;//unused
float Y1;//unused
float X2;//unused
float X3;//unused
float Y2;//unused
float Y3;//unused
int NEval=0;
int NWval=0;
int SEval=0;
int SWval=0;
short armVal;//unused
short wristVal;//unused
short startingArmPos;//unused
short startingWristPos;//unused
short groundWristPos;//unused
short groundArmPos;//unused
bool checkedEnc = false;//unused


task main()
{
	initializeRobot();

	waitForStart();   // wait for start of tele-op phase


	while (true)
	{
		getJoystickSettings(joystick);
		//nMotorPIDSpeedCtrl[DRIVE_NE] = mtrSpeedReg;
		//nMotorPIDSpeedCtrl[DRIVE_NW] = mtrSpeedReg;
		//nMotorPIDSpeedCtrl[DRIVE_SE] = mtrSpeedReg;
		//nMotorPIDSpeedCtrl[DRIVE_SW] = mtrSpeedReg;

		float j1X1 = scaleJoy(joystick.joy1_x1);
		float j1X2 = scaleJoy(joystick.joy1_x2);
		float j1Y1 = scaleJoy(joystick.joy1_y1);

		float j2X2 = scaleJoy(joystick.joy2_x2);//unused
		float j2Y1 = scaleJoy(joystick.joy2_y1);//unused
		float j2Y2 = scaleJoy(joystick.joy2_y2);//unused

				//left x is > left y //mostly <-+->						//right x is > left y
			//left stick x is more than 10 greater than left stick y //right stick x more than 10 greater than left stick y
		if((abs(joystick.joy1_x1) > abs(joystick.joy1_y1)+10) || (abs(joystick.joy1_x2) > abs(joystick.joy1_y1)+10)) {
			NEval =  j1Y1 - j1X2;
			NWval = -j1Y1 + j1X2;
			SEval = -j1Y1 - j1X2;
			SWval =  j1Y1 + j1X2;
		}
			//left stick y is more than 10 more than left stick x
		else if(abs(joystick.joy1_y1)>abs(joystick.joy1_x1)+10) {
			NEval = j1X1 - j1X2;
			NWval = j1X1 + j1X2;
			SEval = j1X1 - j1X2;
			SWval = j1X1 + j1X2;
		}

		else{
			NEval = NWval = SEval= SWval= 0;
		}

		//arm and lift -> arm is joy2y1, lift is joy2y2


		motor[DRIVE_NE]=NEval;
		motor[DRIVE_NW]=NWval;
		motor[DRIVE_SE]=SEval;
		motor[DRIVE_SW]=SWval;

	}
}
