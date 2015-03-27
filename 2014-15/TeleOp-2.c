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
#pragma config(Servo,  srvo_S2_C4_3,    GRAB3,                tServoStandard)
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

int spout_height = kSpoutOpen;
int spout_var = 1;
int times_popped = 0;
bool grab_state_right = false;
bool grab_lock_right = false;
bool grab_state_left = false;
bool grab_lock_left = false;
bool lock_fork = false;
bool popper_has_been_readied = false;
bool spatula_down_var = false;
int grabbed_alt = 0;
bool grabbed_alt_pressed = false;
float J1X1;
float J1Y1;
float J1X2;

float J2Y2;
short current_joystick;


task spatulaDown()
{
  if (!SPATULA_DOWN) {
    motor[FORK] = -100;
  }
  spatula_down_var = false;
  lock_fork = true;
  motor[FORK] = -100;
  while (!spatula_down_var) {
    while (!SPATULA_DOWN) {}
    if (SPATULA_DOWN || SPATULA_UP) {
      spatula_down_var = true;
    } else {
      spatula_down_var = false;
      motor[FORK] = 100;
      while (SPATULA_DOWN) {}
      motor[FORK] = 0;
    }
  }
  if (!SPATULA_UP) {
    motor[FORK] = 100;
    while (SPATULA_DOWN) {}
    motor[FORK] = 0;
  }
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
  servo[SPOUT] = spout_height;
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

    action_joy1
    state bY down
      if (grabbed_alt_pressed == false && grabbed_alt == false) {
          grabbed_alt = 1;
          grabbed_alt_pressed = true;
        } else if (grabbed_alt_pressed == false && grabbed_alt == true) {
          grabbed_alt = 0;
          grabbed_alt_pressed = true;
        }
    otherwise
      grabbed_alt_pressed = false;
    end

    if (grabbed_alt == 1) {
      servo[GRAB3] = kGrab3Closed;
    } else if (grabbed_alt == 0) {
      servo[GRAB3] = kGrab3Open;
    } else {
      servo[GRAB3] = 255;
    }

    if (lock_fork == false) {
      action_joy1 // Rolling Goal Forklift
      state bRB down
        if (!SPATULA_UP) {
          motor[FORK] = 100;
        } else {
          motor[FORK] = 0;
        }
      state bRT down
        if (!SPATULA_DOWN) {
          motor[FORK] = -100;
        } else {
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

    if (grab_state_left) {
      motor[LEFT_LIGHT] = 100;
    } else {
      motor[LEFT_LIGHT] = 0;
    }

    if (grab_state_right) {
      motor[RIGHT_LIGHT] = 100;
    } else {
      motor[RIGHT_LIGHT] = 0;
    }


    if (RIGHT_GRABBER_SWITCH) { // If limit switches are active
      if (!grab_lock_right) { // Unless auto-grabbing has already happened
        grab_lock_right = true; // Prevent auto-grabbing from occurring repeatedly
        grab_state_right = true; // Engage grabbers
      }
    } else { // If both limit switches have been released
      grab_lock_right = false; // Allow auto-grabbing to occur in the future
    }

    if (LEFT_GRABBER_SWITCH) { // If limit switches are active
      if (!grab_lock_left) { // Unless auto-grabbing has already happened
        grab_lock_left = true; // Prevent auto-grabbing from occurring repeatedly
        grab_state_left = true; // Engage grabbers
      }
    } else { // If both limit switches have been released
      grab_lock_left = false; // Allow auto-grabbing to occur in the future
    }

    action_joy1 // (Manual) grabbing of rolling goals
    state bA down // Close
      grab_state_right = true;
      grab_state_left = true;
    state bB down // Open
      grab_state_right = false;
      grab_state_left = false;
    end

    if (grab_state_left) { //activate reft grabber
      servo[GRAB1] = kGrab1Closed;
    } else {
      servo[GRAB1] = kGrab1Open;
    }
    if (grab_state_right) { //activate right grabber
      servo[GRAB2] = kGrab2Closed;
    } else {
      servo[GRAB2] = kGrab2Open;
    }

    if (!grab_state_left && !grab_state_right) { //if both switches are disengaged
      spout_height = kSpoutSlight;
        if (spout_var == 1) {
          servo[SPOUT] = spout_height;
        }
      } else {
      spout_height = kSpoutOpen;
      if (spout_var == 1) {
        servo[SPOUT] = spout_height;
      }
    }

    action_joy2 //adjusting ball dispenser
    state bRT down // close
      spout_var = 0;
      StartTask(closeRoof);
    state bRB down // open normal
      spout_var = 1;
      StartTask(scoringGoals);
    state bX down // open for center goal
      spout_var = 2;
      StartTask(centerGoal);
    end

    if (!POPPER_PRIMED && !popper_has_been_readied) {
      ClearTimer(T1);
      popper_has_been_readied = true;
      times_popped += 1;
    } else if (POPPER_PRIMED) {
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
      grabbed_alt = 2;
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

    HTSPBsetupIO(HTSPB, 0x100);
//    if (!MAX_REACHED) {
//      nxtDisplayTextLine(1, "Magnet absent");
//      //HTSPBwriteIO(HTSPB, 0x10);
//    } else {
//      nxtDisplayTextLine(1, "Magnet present");
//      //HTSPBwriteIO(HTSPB, 0x00);
//    }

    if (!MAX_REACHED || J2Y2 < 0) {
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
