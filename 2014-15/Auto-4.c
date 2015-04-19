#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTMotor,  HTMotor,  HTServo,  HTServo)
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
#pragma config(Servo,  srvo_S2_C4_3,    GRAB3 ,                tServoNone)
#pragma config(Servo,  srvo_S2_C4_4,    servo10,              tServoNone)
#pragma config(Servo,  srvo_S2_C4_5,    servo11,              tServoNone)
#pragma config(Servo,  srvo_S2_C4_6,    servo12,              tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#ifdef USING_CLION

#include "../headers/clion_1.h"

#endif

//#define DEBUG_ULTRA
//#define DEBUG_NO_POP


#define ULTRA_LEFT_VAL USreadDist(msensor_S4_1)
#define ULTRA_RIGHT_VAL USreadDist(msensor_S4_2)
#define SIDE_TOUCH_S TSreadState(msensor_S4_4)
#define SIDE_TOUCH_N TSreadState(msensor_S4_3)

#include "Joystickdriver.c"

#include "../drivers/hitechnic-sensormux.h"
#include "../drivers/hitechnic-irseeker-v2.h"
#include "../drivers/lego-touch.h"
#include "../drivers/hitechnic-superpro.h"
#include "../drivers/lego-ultrasound.h"

#include "../headers/nav_5.h"
#include "../headers/helpers_1.h"
#include "../headers/servoValues_1.h"
#include "../headers/concurrency_1.h"

//==================  Variables ============================

bool setup_done = true;

bool plan_is_park = false;

//==================  Config Definitions  ==================
typedef enum
{
  kAllianceRed,
  kAllianceBlu
} Alliance_t;

typedef enum
{
  kPlanKick,
  kPlanRamp,
  kPlanHigh,
  kPlanBlock,
  kPlanPark
} Plan_t;
#define DEF_PLAN_STRINGS const string Plan_s[] = {"Kick", "Ramp", "High", "Block", "Park"};
#define timeout 5500

#include "../headers/dialog_2.h"

//==================  Functions  ==================

void initialize_servos()
{
  servoChangeRate[SPOUT] = 5;
  servo[ROOF] = kRoofClosed;
  servo[FLAP] = kFlapClosed;
  servo[SPOUT] = kSpoutClosed;
  servo[GRAB1] = kGrab1Open;
  servo[GRAB2] = kGrab2Open;
  servo[GRAB3] = kGrab3Open;
}

void center_slam(unsigned int change = 0)
{
  while (ServoValue[SPOUT] > (110 - change)) {
    servo[SPOUT] -= 1;
    wait1Msec(10);
  }
  wait1Msec(1500);
  servo[SPOUT] = kSpoutClosed;
}

task tele_setup()
{
  GRAB_OPEN;
  bool setup_done = false;
  bool fork_down = true;
  if (!SPATULA_DOWN) {
    motor[FORK] = -100;
    fork_down = false;
  }
  if (!MAX_REACHED) {
    motor[TUBE] = 100;
  }
  while (!MAX_REACHED) {
    if (SPATULA_DOWN && !fork_down) {
      if (SPATULA_DOWN) {
        motor[FORK] = 0;
        fork_down = true;
      }
    }
  }
  motor[TUBE] = 0;
  while (!fork_down) {
    if (SPATULA_DOWN) {
      motor[FORK] = 0;
      fork_down = true;
    }
  }
  setup_done = true;
  halt();
  wait1Msec(500);
}

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
  halt();
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
void ultra_line_up(int distance, unsigned int speed = 35) {
  if (distance < 0) {
    distance = distance * -1;
       drive_t(N, speed, 0);
  bool left = false;
  bool right = false;
  while (!left || !right) {
  if (ULTRA_LEFT_VAL >= distance) {
    motor[DRIVE_NW] = 0;
    motor[DRIVE_SW] = 0;
    left = true;
  }
  if (ULTRA_RIGHT_VAL >= distance) {
    motor[DRIVE_NE] = 0;
    motor[DRIVE_SE] = 0;
    right = true;
  }
}
  } else {
  drive_t(S, speed, 0);
  bool left = false;
  bool right = false;
  while (!left || !right) {
  if (ULTRA_LEFT_VAL <= distance) {
    motor[DRIVE_NW] = 0;
    motor[DRIVE_SW] = 0;
    left = true;
  }
  if (ULTRA_RIGHT_VAL <= distance) {
    motor[DRIVE_NE] = 0;
    motor[DRIVE_SE] = 0;
    right = true;
  }
}
}
}


void square()
{
  drive_t(E, 50, 0);
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
  drive_e(W, 80, 425); // was 530 until 3 Apr; was 600 until 4 mar
}

int seek_ultra_pos()
{
  int first_1_ULTRA = ULTRA_RIGHT_VAL;
  int second_1_ULTRA = ULTRA_LEFT_VAL;
  wait1Msec(50);
  int first_2_ULTRA = ULTRA_RIGHT_VAL;
  int second_2_ULTRA = ULTRA_LEFT_VAL;
  wait1Msec(50);
  int first_3_ULTRA = ULTRA_RIGHT_VAL;
  int second_3_ULTRA = ULTRA_LEFT_VAL;
  int monolith_position;
  int first_ULTRA = (first_1_ULTRA + first_2_ULTRA + first_3_ULTRA)/3;
  int second_ULTRA = (second_1_ULTRA + second_2_ULTRA + second_3_ULTRA)/3;
  drive_e(S, 60 , 800);
  if (first_ULTRA <= 115 || second_ULTRA <= 115) {
    monolith_position = 3;
  } else if (second_ULTRA >= 230 && first_ULTRA >= 230) {
  int first_1_ULTRA = ULTRA_RIGHT_VAL;
  int second_1_ULTRA = ULTRA_LEFT_VAL;
  wait1Msec(50);
  int first_2_ULTRA = ULTRA_RIGHT_VAL;
  int second_2_ULTRA = ULTRA_LEFT_VAL;
  wait1Msec(50);
  int first_3_ULTRA = ULTRA_RIGHT_VAL;
  int second_3_ULTRA = ULTRA_LEFT_VAL;
  int monolith_position;
  int first_ULTRA = (first_1_ULTRA + first_2_ULTRA + first_3_ULTRA)/3;
  int second_ULTRA = (second_1_ULTRA + second_2_ULTRA + second_3_ULTRA)/3;
    if (first_ULTRA <= 115 || second_ULTRA <= 115) {
    monolith_position = 3;
  } else if (second_ULTRA >= 230 && first_ULTRA >= 230) {
    monolith_position = 2;
  } else {
    monolith_position = 1;
  }
  } else {
    monolith_position = 1;
  }
  writeDebugStreamLine("first: %i, second: %i, first1: %i, second1: %i, first2: %i, second2: %i, first3: %i, second3: %i, pos: %i" , first_ULTRA, second_ULTRA, first_1_ULTRA, second_1_ULTRA, first_2_ULTRA, second_2_ULTRA, first_3_ULTRA, second_3_ULTRA, monolith_position);
#ifdef DEBUG_ULTRA
	writeDebugStreamLine("first: %i, second: %i", first_ULTRA, second_ULTRA);
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
  return monolith_position;
}

void pop_it(int times, bool feeder)
{
#ifndef DEBUG_NO_POP
  bool is_cocked = false;
  if (feeder)
    motor[FEEDER] = 100;

  motor[POPPER] = 100;
  while (times >= 0) {
    if (POPPER_PRIMED && !is_cocked) {
      is_cocked = true;
      times--;
      motor[POPPER] = 0;
      wait1Msec(50);
      motor[POPPER] = 100;
    } else if (!POPPER_PRIMED) {
      is_cocked = false;
    }
  }
#else
	PlayImmediateTone(900,20);
	wait1Msec(1000);
#endif
  motor[POPPER] = 0;
  motor[FEEDER] = 0;
}

//==================  Missions  ==================
void mission_monolith(int monolith_position)
{
  writeDebugStreamLine("%i", monolith_position);

  switch (monolith_position) {
    case 1:
      drive_e(S, 30, 2);
      drive_e(N, 30, 2700);
      drive_t(N, 30, 400);
      drive_t(CW, 70, 1200);
      //wait10Msec(100);
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

void mission_block()
{
  drive_e(S, 100, 100);
  drive_e(N, 30, 10);
  drive_e(CCW, 100, 2000);
  drive_e(CW, 30, 10);
  drive_e(BWD - 45, 100, 6500);
  drive_e(N, 30, 10);
  drive_e(FWD - 45, 100, 4000);
  drive_e(CCW, 100, 375);
  drive_e(CW, 50, 10);
  drive_e(BWD + 30, 100, 4500);
  drive_e(FWD + 30, 100, 4500);
  drive_e(N, 100, 1000);
  drive_e(CCW, 100, 3500);
  StartTask(tele_setup);
  while (!setup_done) {abortTimeslice();}
}

void mission_high(int mono_pos) // Center 120 cm goal
{
  servo[FLAP] = kFlapClosed;
  servo[ROOF] = kRoofClosed;
  wait1Msec(350);
  servo[SPOUT] = kSpoutClosed;
  switch (mono_pos) {
    case 1:
      drive_e(CW, 60, 2000);
      drive_e(E, 88, 300);
      drive_e(N, 60, 3500);
      square();
      wait1Msec(250);
      nMotorEncoder[DRIVE_NE] = 0;
      nMotorEncoder[DRIVE_SE] = 0;
        int ne = 88;
        int se = -88;
        int nw = -88;
        int sw = 88;
      ClearTimer(T2);
      float error;
      while(time1[T2] <= 5000 && (ULTRA_RIGHT_VAL >= 100 || ULTRA_LEFT_VAL >= 100)){
        nMotorEncoder[DRIVE_SW] = 0;
    	  error = (nMotorEncoder[DRIVE_SE] - nMotorEncoder[DRIVE_NE]) * 0.001;
    	  if (error > 12) {
    	    error = 12;
    	  } else if (error < -16) {
    	    error = -16;
    	  }
        //error = k_p * (s / 100.0) * (bearing - targetBearing);
        motor[DRIVE_NE] = ne - error;
        motor[DRIVE_SE] = se - error;
        motor[DRIVE_NW] = nw + error;
        motor[DRIVE_SW] = sw + error;
        wait1Msec(2);

      }
      halt();
      wait1Msec(250);
      ultra_line_up(-25, 20);
      ultra_line_up(-30, 20);
      servo[SPOUT] = kSpoutClosed;
      ultra_line_up(27);
       while (!NINETY_REACHED) {}
      center_slam();
      break;
    case 2:
      drive_e(W, 88, 1800);
      ultra_line_up(45);
      drive_e(W, 88, 400);
      wait1Msec(500);
      drive_e(CW, 60, 700);
      drive_e(W, 60, 600);
      halt();
      ultra_line_up(27);
      drive_e(E, 60, 375);
       while (!NINETY_REACHED) {}
     center_slam(10);
      break;
    case 3:
      ultra_line_up(27);
       while (!NINETY_REACHED) {}
       want_state.tube = STOPPED;
      center_slam();

      break;
  }
  drive_t(E, 88, 0);
  while (ULTRA_LEFT_VAL < 60 && ULTRA_RIGHT_VAL < 60) {}
  halt();
  drive_e(E, 88, 400);
  drive_t(S, 100, 1500);
  wait1Msec(250);
  drive_e(N, 60, 600);
  servo[GRAB1] = kGrab1Open;
  servo[GRAB2] = kGrab2Open;
  servo[GRAB3] = kGrab3Open;
  ClearTimer(T4);   //Alex please look at this.  I'm clearing the timer at change of want_state.
  want_state.fork = DOWN;
  drive_e(E, 60, 400);
  drive_e(CW, 60, 300);
      servo[SPOUT] = kSpoutOpen;
      wait1Msec(500);
      servo[ROOF] = kRoofOpen;
      while (is_state.fork == MOVING) {Sleep(300); }
      motor[FORK] = 0;
  //want_state.fork = DOWN;
}

void mission_ramp()
{
  want_state.tube = SIXTY;
  want_state.fork = DOWN;

  drive_t(S, 40, 600);
  drive_t(N, 1, 500);
  drive_t(S, 2, 500);
  drive_t(N, 1, 300);
  drive_e(S, 20, 1600); //was 1500

  drive_t(S, 20, 400);
  PlayImmediateTone(200, 200);
}

void mission_goal1(bool pointed)
{
  want_state.tube = SIXTY;
  want_state.fork = DOWN;
  if (pointed) {
    drive_e(S, 40, 800); //drive forward and line up as well as swerve to make sure the goal is in the right direction
    square();
    while (is_state.fork == MOVING) {Sleep(300);} //wait for 60 cm height and spatula to be all the way down.
    swerve(-50, 500, 700);
  } else {
    drive_e(S, 40, 500); //drive forward and line up
    square();
    while (is_state.fork == MOVING) {Sleep(300);} //wait for 60 cm height and spatula to be all the way down.
    drive_e(S, 40, 750);
  }
  nMotorEncoder[DRIVE_SW] = 0;
  drive_t(S, 15, 0); // grab and score in first goal
  ClearTimer(T1);
  if (pointed) {
    while ((!LEFT_GRABBER_SWITCH || !RIGHT_GRABBER_SWITCH) && time1[T1] < 900) {abortTimeslice();}
  } else {
    while (!LEFT_GRABBER_SWITCH && !RIGHT_GRABBER_SWITCH && time1[T1] < 850) {abortTimeslice();} //tiemout was 900
  }
  //writeDebugStreamLine("Grabbed 60 at time: %i (TO: 900)", time1[T1]);
  GRAB_CLOSE;
  wait1Msec(300);

  halt();
  int encoder_after_swerve = nMotorEncoder[DRIVE_SW];

  while (is_state.tube == MOVING) {Sleep(300);}

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

  pop_it(2, false);
  wait1Msec(300);

  servo[ROOF] = kRoofClosed;
  wait1Msec(500);
  servo[SPOUT] = kSpoutMiddle;

  motor[FORK] = 100;
  wait1Msec(500);
  motor[FORK] = 0;

  if (!plan_is_park) {
    drive_e(N, 50, encoder_after_swerve);
  } else if (pointed) {
    drive_e(N, 50, 200);
  }
}

void mission_goal2(int pointed)
{
  want_state.tube = NINETY;
  drive_e(W, 100, 500);
  drive_t(CCW, 80, 1500);
  want_state.fork = DOWN;
  GRAB_OPEN;
  drive_e(S, 50, 1200);
  drive_e(N, 50, 1300);
  drive_t(CW, 80, 1300);
  drive_e(E, 100, 300);

  square();
  if (pointed == 1) {
    drive_e(S, 40, 1700);// 1500 too short
    swerve_e(-50, 600, 900);// old/standard swerve but slower now
  } else if (pointed == 2) {
    drive_e(S, 40, 1500);
    swerve_e(-50, 600, 1000);
    //drive_e(W, 100, 300);// new/alt swerve for pointy-pointy
  } else { //flat
    drive_e(S, 30, 1600);
  }

  drive_t(S, 18, 0);
  ClearTimer(T1);
  while (!LEFT_GRABBER_SWITCH && !RIGHT_GRABBER_SWITCH && time1[T1] < ((pointed)? 1000 : 1500)) {abortTimeslice();}
  //writeDebugStreamLine("Grabbed 90 at time: %i (TO: 1000 or 1500)", time1[T1]);
  //if (pointed == 2) {
  //	drive_e(CW, 60, 200);
  //	wait1Msec(1000);
  //	drive_t(S, 20, 0);
  //  ClearTimer(T1);
  //  while ((!LEFT_GRABBER_SWITCH || !RIGHT_GRABBER_SWITCH) && time1[T1] < 500) {abortTimeslice();}
  //}
  GRAB_CLOSE;
  wait1Msec(300);
  halt();
  drive_t(N, 20, 500);
  GRAB_OPEN;
  drive_t(S, 20, 300);
  GRAB_CLOSE;

  motor[FEEDER] = 80;
  servo[ROOF] = kRoofClosed;
  wait1Msec(500);
  servoChangeRate[SPOUT] = 15;
  servo[SPOUT] = kSpoutOpen;
  wait1Msec(1200);
  servo[ROOF] = kRoofOpen;
  wait1Msec(500);
  servo[FLAP] = kFlapOpen;
  servoChangeRate[SPOUT] = 10;
  pop_it(4, true);

  if (pointed) {
    motor[DRIVE_NW] = 90;
    motor[DRIVE_SW] = 90;
    wait1Msec(600);
    halt();
  }

  drive_t(N, 100, 500);
}

void mission_park()
{
  drive_e(W, 100, 220);
  drive_e(CCW, 80, 3800);
  drive_e(E, 70, 280);
  drive_e(N, 40, 1000);
  drive_t(W, 90, 2000);
  drive_e(E, 60, 300); // was 200
  drive_e(N, 40, 400);
  servo[GRAB3] = kGrab3Half;
  drive_e(W, 60, 100);

  nMotorEncoder[DRIVE_NW] = 0;
  ClearTimer(T1);
  motor[DRIVE_NW] = 30;
  motor[DRIVE_SW] = 30;
  while (nMotorEncoder[DRIVE_NW] < 500 && time1[T1] < 500) {abortTimeslice();}
  writeDebugStreamLine("thingy ended at time: %i (TO: 500)", time1[T1]);

  servo[GRAB3] = kGrab3Closed;
  Sleep(200);
  want_state.tube = NINETY;
  drive_e(S, 20, 400);// \.
  drive_e(S, 50, 200);//  |- adds to 800
  drive_e(S, 80, 200);// /'
  drive_e(E, 70, 300);

  Sleep(200);
  drive_e(E, 80, 1800); // strafe

  Sleep(200);
  drive_e(S, 80, 4500); //BIG CROSSING

  Sleep(300);
  drive_e(CCW, 30, 100); // spin
  Sleep(300);

  drive_e(S, 80, 4500);

  GRAB_OPEN;
  Sleep(200);
  drive_e(N, 80, 600);
  drive_e(E, 80, 300);
  drive_e(CW, 60, 5500);
  servo[GRAB3] = kGrab3Open;
  drive_e(N, 30, 500);
}

//==================  Main Task  ==================
task main()
{
  HTSPBsetupIO(HTSPB, 0x100);
  writeDebugStreamLine("---start---");

  Alliance_t cur_alli = kAllianceRed;
  Plan_t cur_plan = kPlanHigh;
  int tubes = 2;
  int point = 1;
  int delay = 0;

  int monolith_position;

  dialog(&cur_plan, &tubes, &point, &delay); // Run Dialog for user input of parameters
  waitForStart();
  ClearTimer(T4); //global `run timer' used by new concurrency code
  wait1Msec(delay * 1000);
  StartTask(background_loop);

  switch (cur_plan) {
    case kPlanRamp: //================== Plan Ramp
      initialize_servos();
      mission_ramp();
      if (tubes > 0)
        mission_goal1(point % 2);
      if (tubes > 1)
        mission_goal2(point ? point - 1 : 0);
      break;

    case kPlanKick: //================== Plan Kick
      /*initialize_servos();
      monolith_position = seek_ir_pos();
      mission_monolith(monolith_position);
      drive_e(CW, 80, 5000);
      motor[TUBE] = 0;*/
      break;

    case kPlanHigh: //================== Plan High
      want_state.tube = NINETY;

      monolith_position = seek_ultra_pos();
      mission_high(monolith_position);
      break;

    case kPlanBlock: //================== Plan Defense
      initialize_servos();
      mission_block();
      break;
    case kPlanPark: //=================== Plan Parking Zone
      plan_is_park = true;
      initialize_servos();
      mission_ramp();
      mission_goal1(point % 2);
      mission_park();
      break;
  }

  halt();

  //==================  Ending  ==================
  halt();
  PlayImmediateTone(200, 200);
  wait1Msec(1000);
}
