#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IR_SENSOR,      sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorB,          left,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     DRIVE_SW,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     DRIVE_SE,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     ARM_ROTATE,    tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C2_2,     ARM,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     DRIVE_NE,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     DRIVE_NW,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     DRIVE_,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     DRIVE_,        tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    AUTON_ROT,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    LIFT,                 tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
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

float X1;
float Y1;
float X2;
float X3;
float Y2;

task main()
{
	initializeRobot();

	waitForStart();   // wait for start of tele-op phase

	while (true)
	{
		servo[AUTON_ROT]=255;
		getJoystickSettings(joystick);
		nMotorPIDSpeedCtrl[ARM_ROTATE] = mtrSpeedReg;
		X1 = scaleJoy(joystick.joy1_x1);
		X2 = scaleJoy(joystick.joy1_x2);
		Y1 = scaleJoy(joystick.joy1_y1);
		X3 = scaleJoy(joystick.joy2_x2);
		Y2 = scaleJoy(joystick.joy2_y1);



		motor[ARM] = Y2 *0.7;

		if (joystick.joy2_Buttons & button6){ //RB
			motor[ARM_ROTATE]=-50;
		}
		else if (joystick.joy2_Buttons & button8){ //RT
			motor[ARM_ROTATE]=50;
		}
		else{
			motor[ARM_ROTATE]=0;
		}

		if (joystick.joy2_Buttons & button5){ //LB
			motor[left]=100;
			motor[right]=100;
		}
		else	if (joystick.joy2_Buttons & button7){ //LT
			motor[left]=-100;
			motor[right]=-100;
		}
		else{
			motor[left]=0;
			motor[right]=0;
		}

		if (joystick.joy2_Buttons & button2){ //A
		//	motor[FLAG]=100;
		}
		else	if (joystick.joy2_Buttons & button4){ //Y
			//motor[FLAG]=-100;
		}
		else{
		//	motor[FLAG]=0;
		}

		if (joystick.joy2_Buttons & button1){ //X
			servo[LIFT]=255;
		}
		else	if (joystick.joy2_Buttons & button3){ //B
			servo[LIFT]=0;
		}
		else{
			servo[LIFT]=127;
		}

		motor[DRIVE_NE] = Y1-X1-X2;
		motor[DRIVE_SE] = Y1+X1-X2;
		motor[DRIVE_NW] = Y1+X1+X2;
		motor[DRIVE_SW] = Y1-X1+X2;

	}
}
