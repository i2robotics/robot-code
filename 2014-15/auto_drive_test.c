#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     HTSPB,          sensorI2CCustom9V)
#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
#pragma config(Motor,  motorB,          left,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorNXT, PIDControl, encoder)
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
#pragma config(Servo,  srvo_S2_C4_3,    GRAB3,                tServoNone)
#pragma config(Servo,  srvo_S2_C4_4,    servo10,              tServoNone)
#pragma config(Servo,  srvo_S2_C4_5,    servo11,              tServoNone)
#pragma config(Servo,  srvo_S2_C4_6,    servo12,              tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "../drivers/hitechnic-sensormux.h"
#include "../drivers/hitechnic-irseeker-v1.h"
#include "../drivers/hitechnic-gyro.h"
#include "../drivers/lego-touch.h"
#include "../drivers/hitechnic-superpro.h"

#define IR_SEEK_VAL HTIRSreadDir(msensor_S4_1)
#define GYRO_VAL HTIRSreadDir(msensor_S4_2)
#define GYRO_CAL HTIRSreadDir(msensor_S4_2)
#define SIDE_TOUCH_S TSreadState(msensor_S4_4)
#define SIDE_TOUCH_N TSreadState(msensor_S4_3)

#include "../headers/scaleJoy_1.h"
#include "../headers/gyro_1.h"
#include "../headers/nav_5.h"
#include "../headers/helpers_1.h"
#include "../headers/servoValues_1.h"

void swerve(int power, unsigned int time_1, unsigned int time_2)
{
  motor[DRIVE_NE] = power;
  motor[DRIVE_SE] = power;
  motor[DRIVE_NW] = 0;
  motor[DRIVE_SW] = 0;
  wait1Msec(time_1);

  motor[DRIVE_NE] = 0;
  motor[DRIVE_SE] = 0;
  motor[DRIVE_NW] = power;
  motor[DRIVE_SW] = power;
  wait1Msec(time_2);
}
void swerve_e(int power, int enc_1, int enc_2)
{
  nMotorEncoder[DRIVE_NE] = 0;
  motor[DRIVE_NE] = power;
  motor[DRIVE_SE] = power;
  motor[DRIVE_NW] = 0;
  motor[DRIVE_SW] = 0;
  while (abs(nMotorEncoder[DRIVE_NE]) < enc_1) {abortTimeslice();}

  nMotorEncoder[DRIVE_SW] = 0;
  motor[DRIVE_NE] = 0;
  motor[DRIVE_SE] = 0;
  motor[DRIVE_NW] = power;
  motor[DRIVE_SW] = power;
  while (abs(nMotorEncoder[DRIVE_SW]) < enc_2) {abortTimeslice();}
  halt();
}
void ultra_line_up(int distance) {
  if (distance < 0) {
    distance = distance * -1;

  } else {

  }


}


void square()
{
  drive_t(E, 88, 0);
  ClearTimer(T1);
  int to = 0;
  while (time1[T1] < 2000) {
    if (SIDE_TOUCH_N == 1) {
      motor[DRIVE_NE] = 0;
      motor[DRIVE_NW] = 0;
      to = 1;
    }
    if (SIDE_TOUCH_S == 1) {
      motor[DRIVE_SE] = 0;
      motor[DRIVE_SW] = 0;
      to = 2;
    }
    if (SIDE_TOUCH_N == 1 && SIDE_TOUCH_S == 1) {
      halt();
      PlayImmediateTone(1500, 200);
      to = 3;
      break;
    }
  }
  wait1Msec(400);
  drive_e(W, 100, 530); // was 600 until 4 mar
}

void swerve_e(int power, int enc_1, int enc_2)
{
  nMotorEncoder[DRIVE_NE] = 0;
  motor[DRIVE_NE] = power;
  motor[DRIVE_SE] = power;
  motor[DRIVE_NW] = 0;
  motor[DRIVE_SW] = 0;
  while (abs(nMotorEncoder[DRIVE_NE]) < enc_1) {}

  nMotorEncoder[DRIVE_NW] = 0;
  motor[DRIVE_NE] = 0;
  motor[DRIVE_SE] = 0;
  motor[DRIVE_NW] = power;
  motor[DRIVE_SW] = power;
  while (abs(nMotorEncoder[DRIVE_SW]) < enc_2) {writeDebugStreamLine("%i", nMotorEncoder[DRIVE_SW]);}
  halt();
}


task main()
{
  servo[GRAB3] = kGrab3Half;
  while(true) {}
}
