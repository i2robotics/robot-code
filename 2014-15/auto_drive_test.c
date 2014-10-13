#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorB,          left,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          right,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     DRIVE_NW,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     DRIVE_SW,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     DRIVE_SE,      tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     DRIVE_NE,      tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     LIFT,          tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C3_2,     POPPER,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     b,             tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     a,             tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    ARM_ROT,              tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "../headers/scaleJoy_1.h"
#include "../headers/helpers_1.h"
#include "../headers/nav_4.h"

task main()
{
int speed = 80;
//drive(N, speed, 1000);
//PlayImmediateTone(200,200);
//wait1Msec(500);

drive(NE, speed, 10000);
PlayImmediateTone(200,200);
wait1Msec(500);
drive(E, speed, 1000);
PlayImmediateTone(200,200);
wait1Msec(500);
drive(SE, speed, 1000);
PlayImmediateTone(200,200);
wait1Msec(500);
drive(S, speed, 1000);
PlayImmediateTone(200,200);
wait1Msec(500);
drive(SW, speed, 1000);
PlayImmediateTone(200,200);
wait1Msec(500);
drive(W, speed, 1000);
PlayImmediateTone(200,200);
wait1Msec(500);
drive(NW, speed, 1000);
PlayImmediateTone(200,200);
wait1Msec(500);

}
