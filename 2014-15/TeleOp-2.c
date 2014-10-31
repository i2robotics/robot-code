#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
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
#pragma config(Servo,  srvo_S2_C3_4,    roof,                 tServoNone)
#pragma config(Servo,  srvo_S2_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "../headers/scaleJoy_1.h"
#include "../headers/helpers_1.h"
//#include "../headers/gyro_1.h"

float X1;
float Y1;
float X2;
float Y2;
short current_joystick;
task main()
{
    //nMotorEncoder[DRIVE_SE] = 0;
   // nMotorEncoder[DRIVE_SW] = 0;
	//StartTask(updateBearing);
  while (true) {
    getJoystickSettings(joystick);
    X1 = scaleJoy(joystick.joy1_x1);
    X2 = scaleJoy(joystick.joy1_x2);
    Y1 = scaleJoy(joystick.joy1_y1);
    Y2 = scaleJoy(joystick.joy2_y2);



    motor[LIFT] = Y2;
    motor[DRIVE_NE] = Y1 - X1 - X2;
    motor[DRIVE_SE] = Y1 + X1 - X2;
    motor[DRIVE_NW] = Y1 + X1 + X2;
    motor[DRIVE_SW] = Y1 - X1 + X2;
    writeDebugStreamLine("Y1:%g, X1:%g, X2:%g", Y1, X1, X2);

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
      servo[grab1] = 240;
      servo[grab2]=6;
    state bA down //release
      servo[grab1]=16;
      servo[grab2]=232;
    end

    action_joy2
    state bRB down
      servo[flip] = 0;
    state bRT down
      servo[flip]=255;
    end

    action_joy2
    state bSTART down
      servo[roof] = 200;
    state bBACK down
      servo[roof]=0;
    end

    action_joy2
    state bB down
      motor[POPPER] = 100;
    otherwise
      motor[POPPER] = 0;
    end
  }
}
