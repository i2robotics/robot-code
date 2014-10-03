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
#pragma config(Servo,  srvo_S1_C3_1,    FLAG_ROT,             tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_2,    AUTO_BLK,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Autonomous Mode Code Template
//
// This file contains a template for simplified creation of an autonomous program for an Tetrix robot
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "./nav_2.00.h"  //Our
#include "./scaleJoy.h"  //Our own code to scale joystick values.

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of autonomous mode, you may want to perform some initialization on your robot.
// Things that might be performed during initialization include:
//   1. Move motors and servos to a preset position.
//   2. Some sensor types take a short while to reach stable values during which time it is best that
//      robot is not moving. For example, gyro sensor needs a few seconds to obtain the background
//      "bias" value.
//
// In many cases, you may not have to add any code to this function and it will remain "empty".
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
// The following is the main code for the autonomous robot operation. Customize as appropriate for
// your specific robot.
//
// The types of things you might do during the autonomous phase (for the 2008-9 FTC competition)
// are:
//
//   1. Have the robot follow a line on the game field until it reaches one of the puck storage
//      areas.
//   2. Load pucks into the robot from the storage bin.
//   3. Stop the robot and wait for autonomous phase to end.
//
// This simple template does nothing except play a periodic tone every few seconds.
//
// At the end of the autonomous period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
short encoderVal;
short coolVal;

task main()
{

	initializeRobot();

	// while(true){
	// encoderVal=nMotorEncoder[DRIVE_NW];
	//}
	waitForStart(); // Wait for the beginning of autonomous phase.
	wait10Msec(1000);
	drive(N,30,850);
	drive(W,30,1800);
	nMotorEncoder[DRIVE_NW]=0;
	encoderVal=nMotorEncoder[DRIVE_NW];
	while (SensorValue[IR_SEEK]!=5) {
		if (SensorValue[IR_SEEK]>5 || SensorValue[IR_SEEK]==0) {
			drive(N,20,0);
		}
		else {
			drive(S,20,0);
		}
	}

	drive(S,20,175);

	encoderVal=nMotorEncoder[DRIVE_NW];

	if (encoderVal<=1200) {
		drive(E,30,800);
	}
	else {
		drive(E,30,1600);
	}
	drive(W,30,300);
	halt();



	servo[servo2]=0;
	wait10Msec(70);
	servo[servo2]=255;
	wait10Msec(70);

	drive(W,30,1050);

	nMotorEncoder[DRIVE_NW]=0;

while (coolVal<6750) {
			drive(N,20,0);
			coolVal = encoderVal + nMotorEncoder[DRIVE_NW];
		}
		drive(E,60,400);
		drive(N,60,200);
		drive(E,60,300);
		drive(N,60,150);

		drive(E,60,2000);
		drive(S,80,1600);
}
