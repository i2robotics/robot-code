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

#define DEBUG_IR

#define IR_SEEK_VAL HTIRS2readACDir(msensor_S4_1)
#define GYRO_VAL HTGYROreadRot(msensor_S4_2)
#define SIDE_TOUCH_S TSreadState(msensor_S4_4)
#define SIDE_TOUCH_N TSreadState(msensor_S4_3)

#include "Joystickdriver.c"

#include "../drivers/hitechnic-sensormux.h"
#include "../drivers/hitechnic-irseeker-v2.h"
#include "../drivers/lego-touch.h"
#include "../drivers/hitechnic-superpro.h"

#include "../headers/scaleJoy_1.h"
#include "../headers/gyro_1.h"
#include "../headers/nav_5.h"
#include "../headers/helpers_1.h"
#include "../headers/servoValues_1.h"
//==================  Variables ============================

bool lockout_medium = false;

//==================  Config Definitions  ==================
typedef enum
{
  kAllianceRed,
  kAllianceBlu
} Alliance_t;

typedef enum
{
  kPlanKick,
  kPlanRamp
} Plan_t;
#define DEF_PLAN_STRINGS const string Plan_s[] = {"Kick", "Ramp"};


#include "../headers/dialog_2.h"

void initialize_servos()
{
  servo[ROOF]  = kRoofClosed;
  servo[FLAP]  = kFlapClosed;
  servo[SPOUT] = kSpoutClosed;
  servo[GRAB1] = kGrab1Open;
  servo[GRAB2] = kGrab2Open;
}

task initialize_motors()
{
	bool first_time_repeat = true;
  wait1Msec(800);
  motor[FORK] = -100;
  //start timer
  ClearTimer(T2);
  time1[T2] = 0;
	while ((SPATULA_DOWN & 0x08) != 8) {/*when timer is greater than or equal to the time to get down the ramp clear the timer and start lifting the tube*/
		if (first_time_repeat == true) {
			motor[TUBE] = 100;
			first_time_repeat = false;
			lockout_medium = true;
		}
	}
	//when timer is greater than or equal to 7000 miliseconds stop the lift
	motor[FORK] = 0;

  while (time1[T2] < 7500) {}
  motor[TUBE] = 0;
  ClearTimer(T2);
  lockout_medium = false;
}

task tube_to_top()
{
  motor[TUBE] = 100;
  while (HTSPBreadIO(HTSPB, 0x01) != 1) {}
  motor[TUBE] = 0;
}

void swerve(int  power, unsigned byte cycles, unsigned int time = 400)
{
	for(unsigned byte i = 0; i < cycles; i++) {
		motor[DRIVE_NE] = power;
    motor[DRIVE_SE] = power;
    motor[DRIVE_NW] = 0;
    motor[DRIVE_SW] = 0;
    wait1Msec(400);

    motor[DRIVE_NE] = 0;
    motor[DRIVE_SE] = 0;
    motor[DRIVE_NW] = power;
    motor[DRIVE_SW] = power;
    wait1Msec(400);
  }
}

void square()
{
	drive_t(E, 88, 0);
  ClearTimer(T1);
  time1[T1] = 0;
  while (true) { //time1[T1] < 9000
  	if(SIDE_TOUCH_N == 1) {
  		motor[DRIVE_NE] = 0;
      motor[DRIVE_NW] = 0;
    }
    if (SIDE_TOUCH_S == 1) {
      motor[DRIVE_SE] = 0;
      motor[DRIVE_SW] = 0;
    }
    if (SIDE_TOUCH_N == 1 && SIDE_TOUCH_S == 1) {
    	halt();
    	PlayImmediateTone(1500, 200);
    	break;
    }
  }
  wait1Msec(400);
  drive_e(W, 100, 600); // was 750 worked
}


//==================  Missions  ==================
void mission_monolith(int monolith_position)
{
  writeDebugStreamLine("%i", monolith_position);

  switch (monolith_position) {
    case 1:
      drive_e(N, 30, 2500);
      drive_t(CW, 70, 1200);
      wait10Msec(100);
      drive_e(N, 70, 3000);
      drive_t(S, -2, 1200);
      break;
    case 2:
      drive_e(N, 55, 840);
      drive_t(FWD + 23, 55, 1200);
      drive_e(CW, 100, 1000);
      drive_e(N, 100, 2800);
      drive_t(S, -2, 1200);
      break;
    case 3:
      drive_e(W, 100, 2200);
      drive_e(N, 55, 2600);
      drive_t(ACW, 70, 250);
      drive_e(N, 55, 900);
      drive_t(CW, 40, 1400);
      drive_e(N, 58, 2000);
      //drive_t(S, -20, 1800);
      break;
  }
}

void mission30(int monolith_position)
{
	switch (monolith_position) {
	case 3:
	drive_t(CCW, 70, 1000);
	drive_t(S, 90, 3000);
	drive_t(S, 25, 1500);
	drive_e(N, 90, 300);
	drive_t(CW, 90, 1500);
	 motor[FORK] = -100;
  wait1Msec(4200);
  motor[FORK] = 0;
  ClearTimer(T1);
  while (LEFT_GRABBER_SWITCH == 0 && RIGHT_GRABBER_SWITCH == 0 && time1[T1] < 1000) {}
  servo[GRAB1] = kGrab1Closed;
  servo[GRAB2] = kGrab2Closed;
  wait1Msec(300);
  halt();

	break;
	}
}

void missionCenter(int monolith_position)
{
	switch (monolith_position) {
	case 3:
	drive_t(CCW, 70,200);
	drive_e(N,90,5200);
	drive_t(CCW, 70,500);
	drive_e(S, 50, 1600);
	drive_t(W, 90,500);
	drive_t(CW, 70,200);
	drive_t(W, 90,450);
	//drive_t(CW, 70,100);
	//drive_e(W, 70, 2000);
	//drive_e(S, 70, 300);
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
  motor[POPPER]=100;
  wait1Msec(3000);
  motor[POPPER]=0;

	break;
	}
}

void mission_ramp()
{
	int start_bearing = bearing;
  drive_t(S, 40, 600);
  drive_t(N, 1, 500);
  drive_t(S, 2, 500);
  drive_t(N, 1, 300);
  drive_t(S, 20, 1000);
  drive_e(S, 24, 1000);

  wait10Msec(100);
  //drive_e(CW, 40, 300);

  drive_t(S, 20, 300);
  //go_to_bearing(start_bearing);
  PlayImmediateTone(200, 200);
}

void mission_goal1(bool pointed) //Get the ball in the first goal
{

	if (pointed) {
    drive_t(S, 40, 300); //drive forward and line up as well as swerve to make sure the goal is in the right direction
    square();
    swerve(-90, 2);
  } else {
    drive_t(S, 40, 300); //drive forward and line up
    square();
    drive_t(S, 40, 700);
  }

  drive_t(S, 20, 0); // grab and score in first goal
  ClearTimer(T1);
  while (LEFT_GRABBER_SWITCH == 0 && RIGHT_GRABBER_SWITCH == 0 && time1[T1] < 1000) {}
  servo[GRAB1] = kGrab1Closed;
  servo[GRAB2] = kGrab2Closed;
  wait1Msec(300);
  halt();
  while (lockout_medium == true) {}
  wait1Msec(500);
  servo[SPOUT] = kSpoutOpen;
  wait1Msec(1200);
  servo[ROOF] = kRoofOpen;
  wait1Msec(500);
  servo[FLAP] = kFlapOpen;

  motor[POPPER] = 100;
  wait1Msec(3000);
  motor[POPPER] = 0;
  wait1Msec(300);

  servo[ROOF] = kRoofClosed;
  wait1Msec(500);
  servo[SPOUT] = kSpoutMiddle;

  motor[FORK] = 100;
  wait1Msec(500);
  motor[FORK] = 0;

}

void mission_goal2(bool variant)
{
	StartTask(tube_to_top);
	drive_e(W, 100, 300);
  drive_t(CCW, 100, 1500);
  ClearTimer(T2);
  motor[FORK] = -100;
  while ((SPATULA_DOWN & 0x08) != 8 && time1[T2] < 600) {}
  motor[FORK] = 0;
  servo[GRAB1] = kGrab1Open;
  servo[GRAB2] = kGrab2Open;
  drive_e(S, 50, 1200);
  drive_e(N, 50, 1300);
  drive_t(CW, 100, 1300);

  PlayImmediateTone(200,200);
  //wait1Msec(2000);

  drive_t(E, 100, 300);
	//wait1Msec(2000);
  square();
  //wait1Msec(5000);
  motor[FEEDER] = 80;
  drive_t(S, 30, 0);//.
  ClearTimer(T1);
  while (LEFT_GRABBER_SWITCH == 0 && RIGHT_GRABBER_SWITCH == 0 && time1[T1] < 4000) {}
  servo[GRAB1] = kGrab1Closed;
  servo[GRAB2] = kGrab2Closed;
  halt();

  servo[ROOF] = kRoofClosed;
  wait1Msec(500);
  servo[SPOUT] = kSpoutOpen;
  wait1Msec(1200);
  servo[ROOF] = kRoofOpen;
  wait1Msec(500);
  servo[FLAP] = kFlapOpen;

  motor[POPPER] = 100;
  wait1Msec(2000);
  motor[POPPER] = 0;
  motor[FEEDER] = 0;
  drive_t(N, 100, 500); //.
}

//==================  Main Task  ==================
task main()
{
  HTSPBsetupIO(HTSPB, 0x10);

  Alliance_t cur_alli = kAllianceRed;
  Plan_t cur_plan = kPlanRamp;
  int tubes = 2;
  int delay = 0;

  //StartTask(verify_smux);
  //dialog(&cur_alli, &cur_plan, &tubes, &delay); // Run Dialog for user input of parameters
  HTGYROstartCal(msensor_S4_2);
  initialize_servos();
  //waitForStart();
  //StartTask(updateBearing);
  wait1Msec(delay * 1000);
  wait1Msec(500);

  switch (cur_plan) {
    case kPlanRamp: //================== Plan Ramp
      StartTask(initialize_motors);

      mission_ramp();
      if(tubes) {
        mission_goal1(false);
        mission_goal2(false);
      }
      break;


    case kPlanKick: //================== Plan Kick
      int first_IR = IR_SEEK_VAL;
      while (first_IR > 10) {
				writeDebugStreamLine("bad: %i", first_IR);
      	first_IR = IR_SEEK_VAL;
      }
      drive_e(N, 20, 2000);
      int second_IR = IR_SEEK_VAL;
			while (second_IR > 10) {
				writeDebugStreamLine("bad: %i", second_IR);
      	second_IR = IR_SEEK_VAL;
      }

      int monolith_position;
      if (first_IR <= 3) {
        monolith_position = 1;
      } else if (second_IR == 5) {
        monolith_position = 3;
      } else {
        monolith_position = 2;
      }
#ifdef DEBUG_IR
      writeDebugStreamLine("first: %i, second: %i", first_IR, second_IR);
      writeDebugStreamLine("result: %i", monolith_position);
      switch (monolith_position) {
        case 1:
          PlayImmediateTone(900, 300);
          break;
        case 2:
          PlayImmediateTone(650, 190);
          wait1Msec(200);
          PlayImmediateTone(650, 190);
          wait1Msec(200);
          break;
        case 3:
          PlayImmediateTone(400, 90);
          wait1Msec(100);
          PlayImmediateTone(400, 90);
          wait1Msec(100);
          PlayImmediateTone(400, 90);
          wait1Msec(100);
          break;
      }
      wait1Msec(2000);
#endif
      mission_monolith(monolith_position);
      drive_e(CW, 80, 5000);
      motor[TUBE] = 0;
      missionCenter(monolith_position);
      break;
  }
  halt();

  //==================  Ending  ==================
  halt();
  PlayImmediateTone(200, 200);
  wait1Msec(1000);
}
