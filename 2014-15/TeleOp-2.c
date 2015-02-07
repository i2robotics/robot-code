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

#ifdef USING_CLION
#include "../headers/clion_1.h"
#endif

#include "JoystickDriver.c"
#include "../headers/scaleJoy_1.h"
#include "../headers/helpers_1.h"
#include "../drivers/hitechnic-superpro.h"
#include "../headers/servoValues_1.h"

int times_popped = 0;
bool grab_state = false;
bool grab_lock = false;
bool lock_fork = false;
bool popper_has_been_readied = false;
bool spatula_down_var = false;
float J1X1;
float J1Y1;
float J1X2;

float J2Y2;
short current_joystick;



task spatulaDown() {
	spatula_down_var = false;
	lock_fork = true;
	motor[FORK] = -100;
	while (!spatula_down_var) {
	while (!(SPATULA_DOWN & 0x08)) {}
	if (SPATULA_DOWN & 0x08) {
    	spatula_down_var = true;
    } else {
    	spatula_down_var = false;
  	}
	}
	motor[FORK] = 0;
	lock_fork = false;
}
task closeRoof()
{
  servo[FLAP] = kFlapClosed;
  servo[ROOF] = kRoofClosed;
  wait1Msec(350);
  servo[SPOUT] = kSpoutClosed;
}

task scoringGoals()
{
	servo[FLAP] = kFlapClosed;
  if (ServoValue[ROOF] != kRoofClosed) {
    servo[FLAP] = kFlapClosed;
    servo[ROOF] = kRoofClosed;
    wait1Msec(350);
  }
 servo[SPOUT] = kSpoutOpen;
  wait1Msec(1000);
  servo[ROOF] = kRoofOpen;
  wait1Msec(250);
  servo[FLAP] = kFlapOpen;
}

task centerGoal()
{
  if (ServoValue[SPOUT] != kSpoutClosed) {
    servo[FLAP] = kFlapClosed;
    servo[ROOF] = kRoofClosed;
    wait1Msec(350);
    servo[SPOUT] = kSpoutClosed;
    wait1Msec(1000);
  }
  servo[ROOF] = kRoofHigh;
  wait1Msec(250);
  servo[FLAP] = kFlapHigh;
}

task main()
{
	ClearTimer(T1);
  StartTask(scoringGoals);
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
    action_joy1 // Feeder
    state bLB down
      motor[FEEDER] = -100;
    state bLT down
      motor[FEEDER] = 100;
    otherwise
      motor[FEEDER] = 0;
    end

    action_joy2
    state bSTART down
    	servo[SPOUT] = ServoValue[SPOUT] - 2;
    state bBACK down
   	  servo[SPOUT] = ServoValue[SPOUT] + 2;
		end
    if (lock_fork == false) {
    action_joy1 // Rolling Goal Forklift
    state bRB down
      motor[FORK] = 100;
    state bRT down
		if ((SPATULA_DOWN & 0x08) != 8) {
    		motor[FORK] = -100;
    		motor[LEFT_LIGHT] = 100;
    		motor[RIGHT_LIGHT] = 0;
    	} else {
    		motor[RIGHT_LIGHT] = 100;
    		motor[LEFT_LIGHT] = 0;
    		motor[FORK] = 0;
    	}
    otherwise
      motor[FORK] = 0;
    end
  	}
    action_joy1
    state bX down
    	StartTask(spatulaDown);
    end

    if (LEFT_GRABBER_SWITCH != 0) {
      motor[LEFT_LIGHT] = 100;
    } else {
      motor[LEFT_LIGHT] = 0;
    }

    if (RIGHT_GRABBER_SWITCH != 0) {
      motor[RIGHT_LIGHT] = 100;
    } else {
      motor[RIGHT_LIGHT] = 0;
    }


    if (LEFT_GRABBER_SWITCH != 0 && RIGHT_GRABBER_SWITCH != 0) { // If limit switches are active
      if (!grab_lock) { // Unless auto-grabbing has already happened
        grab_lock = true; // Prevent auto-grabbing from occurring repeatedly
        grab_state = true; // Engage grabbers
      }
    } else { // If both limit switches have been released
      grab_lock = false; // Allow auto-grabbing to occur in the future
    }

    action_joy1 // (Manual) grabbing of rolling goals
    state bA down // Close
      grab_state = true;
    state bB down // Open
      grab_state = false;
    end

    if (grab_state == true) { //activate servos
      servo[GRAB1] = 215;
      servo[GRAB2] = 60;
    } else if (grab_state == false) {
      servo[GRAB1] = 55;
      servo[GRAB2] = 215;
    }

    action_joy2 //adjusting ball dispenser
    state bRT down // close
      StartTask(closeRoof);
    state bRB down // open normal
      StartTask(scoringGoals);
    state bX down // open for center goal
      StartTask(centerGoal);
    end

    if (POPPER_PRIMED == 0 && !popper_has_been_readied) {
    	ClearTimer(T1);
    	popper_has_been_readied = true;
    	times_popped += 1;
  	} else if (POPPER_PRIMED != 0) {
  	  popper_has_been_readied = false;
  	}

  	nxtDisplayTextLine(1, "SE:%i", times_popped);

    action_joy2 // run popper
    state bB && time1[T1] >= 50 down
      motor[POPPER] = 100;
    otherwise
      motor[POPPER] = 0;
    end


    action_joy2 // manually adjust flap
    state bA down
      servo[FLAP] = ServoValue[FLAP] - 2;
    state bY down
      servo[FLAP] = ServoValue[FLAP] + 2;
    end

    action_joy2 // manually adjust roof
    state bLB down
      servo[ROOF] = ServoValue[ROOF] - 2;
    state bLT down
      servo[ROOF] = ServoValue[ROOF] + 2;
    end

    //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//

    HTSPBsetupIO(HTSPB, 0x40);
//    if (LIFT_SWITCH != 1) {
//      nxtDisplayTextLine(1, "Magnet absent");
//      //HTSPBwriteIO(HTSPB, 0x10);
//    } else {
//      nxtDisplayTextLine(1, "Magnet present");
//      //HTSPBwriteIO(HTSPB, 0x00);
//    }

    if (LIFT_SWITCH != 1 || J2Y2 < 0) {
      motor[TUBE] = J2Y2;
    } else {
      motor[TUBE] = 0;
    }

    motor[DRIVE_NE] = (J1Y1 + J1X1 - J1X2);
    motor[DRIVE_SE] = (J1Y1 - J1X1 - J1X2);
    motor[DRIVE_NW] = (J1Y1 - J1X1 + J1X2);
    motor[DRIVE_SW] = (J1Y1 + J1X1 + J1X2);
#ifdef DEBUG_DRIVING
    writeDebugStreamLine("Y1:%g, X1:%g, X2:%g", J1Y1, J1X1, J1X2);
#endif
  }
}
