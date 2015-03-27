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
#pragma config(Servo,  srvo_S2_C4_3,    GRAB3,                tServoNone)
#pragma config(Servo,  srvo_S2_C4_4,    servo10,              tServoNone)
#pragma config(Servo,  srvo_S2_C4_5,    servo11,              tServoNone)
#pragma config(Servo,  srvo_S2_C4_6,    servo12,              tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#ifdef USING_CLION

#include "../headers/clion_1.h"

#endif

//#define DEBUG_IR
//#define DEBUG_NO_POP


#define IR_SEEK_VAL HTIRS2readACDir(msensor_S4_1)
#define ULTRA_VAL USreadDist(msensor_S4_2)
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

bool lockout_medium = false;
bool setup_done = true;
bool lockout_fork = false;

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

task initialize_motors()
{
  bool first_time_repeat = true;
  bool check_spatula = false;
  lockout_medium = true;
  lockout_fork = true;
  motor[TUBE] = 100;


  wait1Msec(800);
  motor[FORK] = -100;
  //start timer
  ClearTimer(T2);
  time1[T2] = 0;

  while (!check_spatula) { // This mess is here to mitigate against flickering values
    if (time1[T2] > timeout || SPATULA_UP) {
      motor[FORK] = 0;
      break;
    }
    while (!SPATULA_DOWN) {//when timer is greater than or equal to the time to get down the ramp clear the timer and start lifting the tube
      if (time1[T2] > timeout || SPATULA_UP) {
        motor[FORK] = 0;
        break;
      }
      if (SIXTY_REACHED) {
        motor[TUBE] = 0;
      }
    }
    if (SPATULA_DOWN) {
      check_spatula = true;
    } else {
      check_spatula = false;
    }
  }

  //when timer is greater than or equal to 7000 miliseconds stop the lift
  motor[FORK] = 0;
  motor[FORK] = 100;
  while (SPATULA_DOWN) {
    if (time1[T2] > timeout || SPATULA_UP) {
      motor[FORK] = 0;
      break;
    }
    if (SIXTY_REACHED) {
      motor[TUBE] = 0;
    }
  }
  motor[FORK] = 0;
  lockout_fork = false;
  while (time1[T2] < 9000 && !SIXTY_REACHED) {}
  motor[TUBE] = 0;
  servo[SPOUT] = 0; //TEMPORARY HACK
  wait1Msec(350);
  servo[ROOF] = kRoofOpen;
  wait1Msec(300);
  ClearTimer(T2);
  lockout_medium = false;
}

void put_fork_down()
{
  ClearTimer(T2);
  GRAB_OPEN;
  bool spatula_down_var = false;
  if (!SPATULA_DOWN) {
    motor[FORK] = -100;
  }
  spatula_down_var = false;
  motor[FORK] = -100;
  while (!spatula_down_var) {
    if (time1[T2] > timeout) {
      motor[FORK] = 0;
      break;
    }
    while (!SPATULA_DOWN) {
      if (time1[T2] > timeout) {
        motor[FORK] = 0;
        break;
      }
    }
    if (SPATULA_DOWN) {
      spatula_down_var = true;
    } else {
      spatula_down_var = false;
      motor[FORK] = 100;
      while (SPATULA_DOWN) {}
      motor[FORK] = 0;
    }
  }
  motor[FORK] = 100;
  while (SPATULA_DOWN) {}
  motor[FORK] = 0;
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

task tube_to_top()
{
  if (!MAX_REACHED) {
    motor[TUBE] = 100;
  }
  while (HTSPBreadIO(HTSPB, 0x01) != 1) {}
  motor[TUBE] = 0;
}

void shake()
{
  drive_e(E, 100, 100);
  drive_e(W, 100, 200);
  drive_e(E, 100, 100);
  drive_e(W, 100, 200);
  drive_e(E, 100, 20);
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
  while (abs(nMotorEncoder[DRIVE_NE]) < enc_1) {}

  nMotorEncoder[DRIVE_SW] = 0;
  motor[DRIVE_NE] = 0;
  motor[DRIVE_SE] = 0;
  motor[DRIVE_NW] = power;
  motor[DRIVE_SW] = power;
  while (abs(nMotorEncoder[DRIVE_SW]) < enc_2) {}
  halt();
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

int seek_ir_pos()
{
  int first_IR = IR_SEEK_VAL;
  while (first_IR > 10) {
    first_IR = IR_SEEK_VAL;
  }
  drive_e(S, 40, 2000);
  servo[SPOUT] = kSpoutClosed;
  int second_IR = IR_SEEK_VAL;
  while (second_IR > 10) {
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
  while (!setup_done) {}
}

void mission_high(int mono_pos) // Center 120 cm goal
{
  servo[FLAP] = kFlapClosed;
  servo[ROOF] = kRoofClosed;
  wait1Msec(350);
  servo[SPOUT] = kSpoutClosed;

  if (mono_pos == 3) {
    mono_pos = 2;
  }
  if (mono_pos == 1) {
    int mono_pos2;
    drive_e(CCW, 100, 1000);
    drive_e(S, 50, 1000); //2700
    servo[SPOUT] = kSpoutClosed;
    PlayImmediateTone(1200, 200);
    wait1Msec(200);
    if (IR_SEEK_VAL <= 3) {
      mono_pos2 = 1;
    } else {
      mono_pos2 = 2;
    }
    if (mono_pos2 == 1) {
      drive_e(S, 50, 2800);
      while (!MAX_REACHED) {}
      drive_e(CW, 100, 600);
      drive_e(S, 50, 1200);
      drive_e(CW, 60, 900);
      drive_t(CW, 100, 0);
      while (ULTRA_VAL > 75) {}
      drive_e(CCW, 100, 50);
      halt();
      drive_e(W, 88, 200);
      drive_t(S, 60, 0);
      while (ULTRA_VAL > 29) {}
      halt();
      servoChangeRate[SPOUT] = 15;
      servo[SPOUT] = kSpoutMiddle;
      wait1Msec(2000);
      servoChangeRate[SPOUT] = 10;


      drive_e(N, 60, 500);
      drive_e(CCW, 60, 50);
      drive_e(W, 88, 400);

    } else {
      drive_e(S, 50, 1300);
      drive_e(CW, 100, 1800);
      while (!MAX_REACHED) {}
      servo[SPOUT] = kSpoutClosed;
      drive_t(S, 60, 0);
      while (ULTRA_VAL > 32) {}
      halt();
      drive_e(E, 88, 100);
      //while (true) {}
      servoChangeRate[SPOUT] = 15;
      servo[SPOUT] = 100;
      wait1Msec(1500);
      servoChangeRate[SPOUT] = 10;
      drive_e(N, 40, 350);
      //drive_e(CCW, 100, 1500);
      //drive_e(N, 50, 2700);
      //drive_e(CW, 100, 1000);
    }
  } else {
    servo[SPOUT] = kSpoutClosed;
    while (!MAX_REACHED) {}
    servo[SPOUT] = kSpoutClosed;
    drive_t(S, 60, 0);
    while (ULTRA_VAL > 29) {}
    halt();
    //while (true) {}
    servoChangeRate[SPOUT] = 15;
    servo[SPOUT] = 100;
    wait1Msec(1500);
    servoChangeRate[SPOUT] = 10;
    drive_e(N, 60, 1000);
    drive_e(E, 88, 200);
  }
  drive_e(E, 88, 1800);
  drive_e(S, 100, 1500);
  drive_t(BWD + 20, 100, 1500);
  drive_t(CW, 100, 1000);
  put_fork_down();
}

void mission_ramp()
{
  //	int start_bearing = bearing;
  drive_t(S, 40, 600);
  drive_t(N, 1, 500);
  drive_t(S, 2, 500);
  drive_t(N, 1, 300);
  drive_e(S, 20, 2000);

  drive_t(S, 20, 400);
  PlayImmediateTone(200, 200);
}

void mission_goal1(bool pointed)
{
  if (pointed) {
    drive_e(S, 40, 800); //drive forward and line up as well as swerve to make sure the goal is in the right direction
    square();
    while (lockout_fork) {} //wait for 60 cm height and spatula to be all the way down.
    swerve(-90, 500, 700);
  } else {
    drive_e(S, 40, 500); //drive forward and line up
    square();
    while (lockout_fork) {} //wait for 60 cm height and spatula to be all the way down.
    drive_e(S, 40, 750);
  }
  nMotorEncoder[DRIVE_SW] = 0;
  drive_t(S, 20, 0); // grab and score in first goal
  ClearTimer(T1);
  if (pointed) {
    while ((!LEFT_GRABBER_SWITCH || !RIGHT_GRABBER_SWITCH) && time1[T1] < 900) {}
  } else {
    while (!LEFT_GRABBER_SWITCH && !RIGHT_GRABBER_SWITCH && time1[T1] < 900) {}
  }
  servo[GRAB1] = kGrab1Closed;
  servo[GRAB2] = kGrab2Closed;
  wait1Msec(300);

  halt();
  int encoder_after_swerve = nMotorEncoder[DRIVE_SW];

  while (lockout_medium) {}

  pop_it(2, false);
  wait1Msec(300);

  servo[ROOF] = kRoofClosed;
  wait1Msec(500);
  servo[SPOUT] = kSpoutMiddle;

  motor[FORK] = 100;
  wait1Msec(500);
  motor[FORK] = 0;

  //if (pointed) {
  drive_e(N, 50, encoder_after_swerve);
  //}
}

void mission_goal2(int pointed)
{
  StartTask(tube_to_top);
  drive_e(W, 100, 500);
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
  motor[FORK] = 100;
  while (SPATULA_DOWN) {}
  motor[FORK] = 0;
  drive_t(E, 100, 300);

  square();
  if (pointed == 1) {
    drive_e(S, 40, 2100);// 1500 too short
    swerve_e(-50, 600, 900);// old/standard swerve but slower now
  } else if (pointed == 2) {
    drive_e(S, 40, 1500);
    swerve_e(-50, 600, 1000);
    //drive_e(W, 100, 300);// new/alt swerve for pointy-pointy
  } else { //flat
    drive_e(S, 40, 1600);
    drive_e(E, 100, 200);
  }

  drive_t(S, 15, 0);
  ClearTimer(T1);
  while (!LEFT_GRABBER_SWITCH && !RIGHT_GRABBER_SWITCH && time1[T1] < 1500) {}
  //if (pointed == 2) {
  //	drive_e(CW, 60, 200);
  //	wait1Msec(1000);
  //	drive_t(S, 20, 0);
  //  ClearTimer(T1);
  //  while ((!LEFT_GRABBER_SWITCH || !RIGHT_GRABBER_SWITCH) && time1[T1] < 500) {}
  //}
  GRAB_CLOSE;
  wait1Msec(140);
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

//==================  Main Task  ==================
task main()
{
  HTSPBsetupIO(HTSPB, 0x40);

  Alliance_t cur_alli = kAllianceRed;
  Plan_t cur_plan = kPlanPark;
  int tubes = 2;
  int point = 0;
  int delay = 0;

  int monolith_position;

  //dialog(&cur_plan, &tubes, &point, &delay); // Run Dialog for user input of parameters
  //waitForStart();
  ClearTimer(T4); //global `run timer' used by new concurrency code
  wait1Msec(delay * 1000);

  switch (cur_plan) {
    case kPlanRamp: //================== Plan Ramp
      initialize_servos();
      StartTask(initialize_motors);

      mission_ramp();

      if (tubes > 0)
        mission_goal1(point == 1 || point == 3);
      if (tubes > 1)
        mission_goal2(point ? point - 1 : 0);

      break;

    case kPlanKick: //================== Plan Kick
      initialize_servos();
      monolith_position = seek_ir_pos();
      mission_monolith(monolith_position);
      drive_e(CW, 80, 5000);
      motor[TUBE] = 0;
      break;

    case kPlanHigh: //================== Plan High
      StartTask(tube_to_top);

      monolith_position = seek_ir_pos();
      mission_high(monolith_position);
      break;

    case kPlanBlock: //================== Plan Defense
      initialize_servos();
      mission_block();
      break;
    case kPlanPark: //=================== Plan Parking Zone
      initialize_servos();
      StartTask(initialize_motors);
      mission_ramp();
      mission_goal1(point == 1 || point == 3);
      if (tubes > 0) {

      } else if (tubes > 1) {
        //drive_e(CCW, 60, 3000);

      }
  }

  halt();

  //==================  Ending  ==================
  halt();
  PlayImmediateTone(200, 200);
  wait1Msec(1000);
}
