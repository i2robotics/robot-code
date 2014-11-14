/*--------------------------------------------------
 *
 *    CLion v. 1
 *      RobotC definitions for debugging
 *      in CLion.
 *
 *------------------------------------------------*/

#ifndef CLION_INCLUDED
#define CLION_INCLUDED
#include <Kernel/stdbool.h>
typedef const char *string;
typedef int byte;
typedef void task;

struct cLion_Joystick_t {
  joy1_x1;
  joy1_y1;
  joy1_x2;

  joy2_y2;
  joy1_Buttons;
  joy2_Buttons;
} joystick;

#define DRIVE_NE 0
#define DRIVE_SE 1
#define DRIVE_NW 2
#define DRIVE_SW 3
#define FEEDER 1
#define grab1 1
#define grab2 1
#define roof 1
#define flip 1
#define POPPER 1
#define HTSPB 1
#define LIFT 1
#define IR_SEEK 1

const int T1;
const int T2;
const int externalBatteryAvg;
const int nAvgBatteryLevel;
const int nNxtButtonPressed;
const int time10[1];
int time1[2];
int motor[4];
int servo[6];
int nMotorEncoder[4];
int SensorValue[9];

void ClearTimer(int t) {}
void nxtDisplayStringAt(int x, int y, string string, ...) {}
void nxtDisplayTextLine(int line, string str) {}
void nxtDrawLine(int x1, int y1, int x2, int y2) {}
void PlayImmediateTone(int frequency, int duration) {}
void wait1Msec(int miliseconds) {}
void writeDebugStreamLine(string string, ...) {}
float cos(double number) {}
float sin(double number) {}
void getJoystickSettings(struct cLion_Joystick_t joystick) {}

#endif
