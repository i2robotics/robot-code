#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S3,     HTSMUX,         sensorI2CCustom)
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
#pragma config(Servo,  srvo_S1_C3_5,    ULTRA_ROT,            tServoStandard)
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
#include "drivers/hitechnic-sensormux.h"
#include "drivers/lego-light.h"

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

const tMUXSensor LEGOLS = msensor_S3_1;

task main()
{
	initializeRobot();
	//waitForStart();   // wait for start of tele-op phase

	int raw = 0;
  int nrm = 0;
  bool active = true;

  // Turn the light on
  LSsetActive(LEGOLS);

  nNxtButtonTask  = -2;

  nxtDisplayCenteredTextLine(0, "Lego");
  nxtDisplayCenteredBigTextLine(1, "LIGHT");
  nxtDisplayCenteredTextLine(3, "SMUX Test");
  nxtDisplayCenteredTextLine(5, "Connect SMUX to");
  nxtDisplayCenteredTextLine(6, "S1 and sensor to");
  nxtDisplayCenteredTextLine(7, "SMUX Port 1");
  wait1Msec(2000);

  nxtDisplayClearTextLine(7);
  nxtDisplayTextLine(5, "Press [enter]");
  nxtDisplayTextLine(6, "to toggle light");
  wait1Msec(2000);

  while (true) {
    // The enter button has been pressed, switch
    // to the other mode
    if (nNxtButtonPressed == kEnterButton) {
      active = !active;
      if (!active)
        LSsetInactive(LEGOLS);
      else
        LSsetActive(LEGOLS);

      // wait 500ms to debounce the switch
      wait1Msec(500);
    }

    nxtDisplayClearTextLine(5);
    nxtDisplayClearTextLine(6);
    raw = LSvalRaw(LEGOLS);
    nrm = LSvalNorm(LEGOLS);
    nxtDisplayTextLine(5, "Raw:  %4d", raw);
    nxtDisplayTextLine(6, "Norm: %4d", nrm);
    wait1Msec(50);
  }
}
