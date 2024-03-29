#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
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
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define csfront 	0
#define csback 		1
#define csGray 			1
#define csGrayWhite 2
#define csWhite 		3
#define csGrayBlue 	4
#define csBlue 			5
#define csBlueRed 	6
#define csGrayRed 	7
#define csRed 			8

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Autonomous Mode Code Template
//
// This file contains a template for simplified creation of an autonomous program for an Tetrix robot
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "./nav_2.00.h"  //Our
#include "./scaleJoy.h"  //Our own code to scale joystick values.
#include "drivers/hitechnic-sensormux.h"
#include "drivers/lego-light.h"
#include "drivers/hitechnic-colour-v2.h"


int choiceArray[] = {1, 1, 2};
int delay = 3;
short wheelEnc_2=0;
int wheelEnc_3;
short wheelEnc;
const tMUXSensor HTCOLOR1 = msensor_S3_1;
const tMUXSensor HTCOLOR2 = msensor_S3_2;
int turnSpeed = 23;
short	turnOffset;
int red = 0;
int green = 0;
int blue = 0;

int color(int value) {
	if (value==0) {
		HTCS2readRGB(HTCOLOR1, red, green, blue);
	}
	else if (value==1) {
		HTCS2readRGB(HTCOLOR2, red, green, blue);
	}
	if (red<27 && green<27 && blue < 23) {
		return 1;//gray
	}
	else if (red < 130 && red > 45 && green < 130 && green > 45 && blue > 45 && blue < 120) {
		return 2;//graywhite
	}
	else if (red > 153 && green > 159 && blue > 142) {
		return 3;//white
	}
	else if (red  < 25 && green > 25 && green < 40 && blue < 60 && blue > 50) {
		return 4;
	}
	else if (red < 25 && green > 33 && green < 45 && blue > 70 && blue < 85) {
		return 5;
	}
	else if (red > 53 && red < 68 && green < 35 && green > 20 && blue > 30 && blue < 43) {
		return 6;
	}
	else if (red > 50 && red < 62 && green > 15 && green < 25 && blue <20) {
		return 7;
	}
	else if (red > 90 && red < 110 && green <25 && blue < 10) {
		return 8;
	}
	return 1;
}

bool onWhite(int value) {
	if (color(value) == csWhite) {
		return true;
	}
	return false;
}

bool onGray(int value) {
	if (color(value) == csGray) {
		return true;
	}
	return false;
}

bool onGrayWhite(int value) {
	if (color(value) == csGrayWhite) {
		return true;
	}
	return false;
}

void endPgm() {
	halt();
	PlayImmediateTone(200,200);
	while (true) {}
}


void lineUp() {
	ClearTimer(T1);
	while ( !(onWhite(csfront) || onWhite(csback)) )
	{
		drive(E,50,0);
	}
	drive(W,70,200);
	short counter = 0;

	if (onGray(csfront)){

        while ( ((onGray(csfront)|| onWhite(csfront))  ||  (onGray(csback) || onWhite(csback)) ) && counter<2) {
            halt();
            counter += 1;
            while (onGray(csfront) || onWhite(csfront)){
                if (onGray(csfront)){
                    motor[DRIVE_NE] = -turnSpeed;
                    motor[DRIVE_NW] = turnSpeed;
                    motor[DRIVE_SW] = turnSpeed;
                }
                else{
                    motor[DRIVE_NE] = turnSpeed;
                    motor[DRIVE_NW] = -turnSpeed;
                    motor[DRIVE_SW] = -turnSpeed;
                }
                if (time1[T1]>10000) {
                    endPgm();
                }
            }
            halt();
            while (onGray(csback) || onWhite(csback)) {
                if (onGray(csback)){
                    motor[DRIVE_SE] = turnSpeed;
                    motor[DRIVE_SW] = -turnSpeed;
                    motor[DRIVE_NW] = -turnSpeed;
                }
                else{
                    motor[DRIVE_SE] = -turnSpeed;
                    motor[DRIVE_SW] = turnSpeed;
                    motor[DRIVE_NW] = turnSpeed;
                }
                if (time1[T1]>10000) {
                    endPgm();
                }
            }
        }
    }
    else if (onGray(csback)){

        while (((onGray(csfront)|| onWhite(csfront)) || (onGray(csback) || onWhite(csback))) && counter<2) {
            halt();
            counter += 1;

            while (onGray(csback) || onWhite(csback)) {
                if (onGray(csback)){
                    motor[DRIVE_SE] = turnSpeed;
                    motor[DRIVE_SW] = -turnSpeed;
                    motor[DRIVE_NW] = -turnSpeed;
                }
                else{
                    motor[DRIVE_SE] = -turnSpeed;
                    motor[DRIVE_SW] = turnSpeed;
                    motor[DRIVE_NW] = turnSpeed;
                }
                if (time1[T1]>10000) {
                    endPgm();
                }
            }
            halt();
            while (onGray(csfront) || onWhite(csfront)) {
                if (onGray(csfront)){
                    motor[DRIVE_NE] = -turnSpeed;
                    motor[DRIVE_NW] = turnSpeed;
                    motor[DRIVE_SW] = turnSpeed;
                }
                else{
                    motor[DRIVE_NE] = turnSpeed;
                    motor[DRIVE_NW] = -turnSpeed;
                    motor[DRIVE_SW] = -turnSpeed;
                }
                if (time1[T1]>10000) {
                    endPgm();
                }
            }
        }
    }

}

void initializeRobot()
{
	// Place code here to sinitialize servos to starting positions.
	// Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

	servo[AUTO_BLK]=255;
	servo[FLAG_ROT]=0;
	int selected = 0;
	ClearTimer(T1);
	const char *startChoices[] = {"Left","Right"};
	const char *endChoices[] = {"Left","Right"};
	const char *bridgeChoices[] = {"Close","Far","No"};
	while (true) {
		eraseDisplay();
		nxtDisplayStringAt(15, 62, "Start:");
		nxtDisplayStringAt(15, 52, "End:");
		nxtDisplayStringAt(15, 42, "Bridge:");
		nxtDisplayStringAt(15, 32, "Delay:");
		nxtDrawLine (0, 22, 100, 22);
		nxtDisplayStringAt(5, 18, "Ext Batt: %.2f", ((float) externalBatteryAvg)/1000);
		nxtDisplayStringAt(5, 8, "NXT Batt: %.2f", ((float) nAvgBatteryLevel)/1000);

		nxtDisplayStringAt(55, 62, "%s", startChoices[choiceArray[0]]);
		nxtDisplayStringAt(43, 52, "%s", endChoices[choiceArray[1]]);
		nxtDisplayStringAt(59, 42, "%s", bridgeChoices[choiceArray[2]]);
		nxtDisplayStringAt(55, 32, "%d", delay);
		//sizeof(startChoices) / sizeof(startChoices[0]);
		nxtDisplayStringAt(5, 62 -selected*10, "*");

		if (nNxtButtonPressed == 1) {//button press right
			if (time10[T1]>30) {
				switch (selected) {
					case 0:
						if (choiceArray[0]==1) {
							choiceArray[0] = 0;
						}
						else {
							choiceArray[0]++;
						}
						break;
					case 1:
						if (choiceArray[1]==1) {
							choiceArray[1] = 0;
						}
						else {
							choiceArray[1]++;
						}
						break;
					case 2:
						if (choiceArray[2]==2) {
							choiceArray[2] = 0;
						}
						else {
							choiceArray[2]++;
						}
						break;
					case 3:
						if (delay < 20){
							delay++;
						}
						else {
							delay = 0;
						}
						break;
				}
				ClearTimer(T1);
			}
		}
		if (nNxtButtonPressed == 2) {// on button press Left
			if (time10[T1]>30) {
				if (selected == 3) {
                    selected = 0;
                }
                else {
                    selected++;             //increment selected
                }
                ClearTimer(T1);
            }
		}
		if (nNxtButtonPressed == 3) { // button orange
			break;
		}


		wait10Msec(2);
	}
	return;
}


int speed = 50;

task main()
{
	initializeRobot();
	//waitForStart();
	servo[AUTO_BLK]=255;
	nMotorEncoder[DRIVE_NW] = 0;
	wait10Msec(delay*100);
	ClearTimer(T1);//reset internal timer T1

	if (choiceArray[0]==1) { // if start == 1 || right
        nMotorEncoder[DRIVE_NW]=0;

		while (SensorValue[IR_SEEK]!=5) {
			if (SensorValue[IR_SEEK]>5) {
				drive(S,speed,0);
			}
			else {
				drive(N,speed,0);
			}
		}

		wait10Msec(10);
 		wheelEnc_3 = nMotorEncoder[DRIVE_NW];
		//while (nMotorEncoder[DRIVE_NW] - wheelEnc_3> -200) {
		//	drive(S,20,0);
		//}
		if (nMotorEncoder[DRIVE_NW] > 4500) {
			wheelEnc_3 = nMotorEncoder[DRIVE_NW];
			while ((nMotorEncoder[DRIVE_NW] - wheelEnc_3) > -365) {
				drive(S,20,0);
			}
		}
		else {
			while (nMotorEncoder[DRIVE_NW] - wheelEnc_3< 100) {
				drive(N,20,0);
			}
		}

    	wheelEnc=nMotorEncoder[DRIVE_NW];

		servoChangeRate[AUTO_BLK]=8;

		halt();

		servo[AUTO_BLK]=0;
		wait10Msec(85);
		servoChangeRate[AUTO_BLK]=10;
		servo[AUTO_BLK]=255;
		wait10Msec(40);


		nMotorEncoder[DRIVE_NW] = 0;
	}
	else if (choiceArray[0]==0) { //if start == 0 || left
		nMotorEncoder[DRIVE_NW]=0;

		while (SensorValue[IR_SEEK]!=5) {
			if (SensorValue[IR_SEEK]>5 || SensorValue[IR_SEEK]==0) {
				drive(S,speed,0);
			}
			else {
				drive(N,speed,0);
			}
		}

		drive(S,20,175);
 		wheelEnc_3 = nMotorEncoder[DRIVE_NW];
		//while (nMotorEncoder[DRIVE_NW] - wheelEnc_3> -200) {
		//	drive(S,20,0);
		//}
		if (nMotorEncoder[DRIVE_NW] < -5000) {
			wheelEnc_3 = nMotorEncoder[DRIVE_NW];
			while ((nMotorEncoder[DRIVE_NW] - wheelEnc_3) < 0) {
				drive(N,30,0);
			}
		}
		else{
			while ((nMotorEncoder[DRIVE_NW] - wheelEnc_3) > -300) {
				drive(S,30,0);
			}
		}

		servoChangeRate[AUTO_BLK]=8;

		halt();
    	wheelEnc = nMotorEncoder[DRIVE_NW];//set wheelEnc
    	nMotorEncoder[DRIVE_NW] = 0;			//before resetting it

		servo[AUTO_BLK]=0;
		wait10Msec(85);
		servoChangeRate[AUTO_BLK]=10;
		servo[AUTO_BLK]=255;
		wait10Msec(40);

	}// </start>


	ClearTimer(T1);
	if (choiceArray[1]==0) { //if end == left 0
		if (choiceArray[0]==1){ // if started right
			wheelEnc_2 = wheelEnc; //wheelEnc as at the end of start
			while (wheelEnc_2 < 7650) {
				drive(N,speed,0);
				wheelEnc_2 = wheelEnc + nMotorEncoder[DRIVE_NW];//= encVal at end of start + since this while started
				if (time1[T1]>4000) { //safety
					endPgm();
				}
			}
			halt();
		}
		else if (choiceArray[0]==0) { //if started left
			wheelEnc_2 = wheelEnc;//wheelEnc as at the end of start
			while (wheelEnc_2<-1000) {
				drive(N,speed,0);
				wheelEnc_2 = wheelEnc + nMotorEncoder[DRIVE_NW];//= encVal at end of start + since this while started
				if (time1[T1]>4000) {
					endPgm(); //safety
				}
			}
		}

		if (choiceArray[2]==2) { //if bridge == no light sensors
		nMotorEncoder[DRIVE_NW]=0;
			while(nMotorEncoder[DRIVE_NW]<2050){
				motor[DRIVE_NE]=-100;
				motor[DRIVE_NW]=100;
				motor[DRIVE_SE]=-100;
				motor[DRIVE_SW]=100;
			}
			halt();

			nMotorEncoder[DRIVE_NW]=0;
			while(nMotorEncoder[DRIVE_NW]<4500){
				motor[DRIVE_NE]=100;
				motor[DRIVE_NW]=100;
				motor[DRIVE_SE]=100;
				motor[DRIVE_SW]=100;
			}
			halt();

			nMotorEncoder[DRIVE_NW]=0;
			while(nMotorEncoder[DRIVE_NW]>-1250){
				motor[DRIVE_NE]=100;
				motor[DRIVE_NW]=-100;
				motor[DRIVE_SE]=100;
				motor[DRIVE_SW]=-100;
			}
			halt();


			nMotorEncoder[DRIVE_NW]=0;
			ClearTimer(T1);
			while(nMotorEncoder[DRIVE_NW] > -6000) {// was -6550
				drive(S,100,0);
				if (time1[T1]>5000) {
					endPgm();
				}
			}
		}
		else { //if bridge != none

			turnOffset=nMotorEncoder[DRIVE_NW];
			while(nMotorEncoder[DRIVE_NW]-turnOffset<2050){
				motor[DRIVE_NE]=-100;
				motor[DRIVE_NW]=100;
				motor[DRIVE_SE]=-100;
				motor[DRIVE_SW]=100;
			}
			halt();

			turnOffset=nMotorEncoder[DRIVE_NW];
			while(nMotorEncoder[DRIVE_NW]-turnOffset<3000){
				motor[DRIVE_NE]=100;
				motor[DRIVE_NW]=100;
				motor[DRIVE_SE]=100;
				motor[DRIVE_SW]=100;
			}
			halt();

			turnOffset=nMotorEncoder[DRIVE_NW];
			while(nMotorEncoder[DRIVE_NW]-turnOffset>-1400){
				motor[DRIVE_NE]=100;
				motor[DRIVE_NW]=-100;
				motor[DRIVE_SE]=100;
				motor[DRIVE_SW]=-100;
			}
			halt();
			lineUp();
			drive(E,80,150);
			drive(S,40,300);
			if (choiceArray[2]==1) {// if bridge == far
				drive(E,80,150);
				while (onGray(csfront) && onGray(csback)) {
					drive(E,50,0);
				}
				lineUp();
				drive(E,80,400);
				while (onGray(csfront) && onGray(csback)) {
					drive(E,50,0);
				}

				lineUp();
				drive(W,80,200);
			}
			nMotorEncoder[DRIVE_NW]=0;
			ClearTimer(T1);
			while(nMotorEncoder[DRIVE_NW]>-5500) {
				drive(S,100,0);
				if (time1[T1]>5000) {
					endPgm();
				}
			}
		}


	}
	else if (choiceArray[1]==1) {// if end == right
		ClearTimer(T1);
		if (choiceArray[0]==1){// if started right
			wheelEnc_2 = wheelEnc;//wheelEnc as at the end of start
			while (wheelEnc_2>850) {//was 1025
				drive(S,speed,0);
				wheelEnc_2 = wheelEnc + nMotorEncoder[DRIVE_NW];//= encVal at end of start + since this while started
				if (time1[T1]>4000) {//safety
					endPgm();
				}
			}
			halt();
		}
		else if (choiceArray[0]==0) {// if started left
			wheelEnc_2 = wheelEnc;//wheelEnc as at the end of start
			while (wheelEnc_2>-7850) {
				drive(S,speed,0);
				wheelEnc_2 = wheelEnc + nMotorEncoder[DRIVE_NW];//= encVal at end of start + since this while started
				if (time1[T1]>4000) {
					endPgm();//safety
				}
			}
		}

		if (choiceArray[2]==2) {// if bridge == none
			nMotorEncoder[DRIVE_NW]=0;
			while(nMotorEncoder[DRIVE_NW]>-3000){//was -1800
				motor[DRIVE_NE]=100;
				motor[DRIVE_NW]=-100;
				motor[DRIVE_SE]=100;
				motor[DRIVE_SW]=-100;
			}
			halt();

			nMotorEncoder[DRIVE_NW]=0;
			while(nMotorEncoder[DRIVE_NW]>-4500){
				motor[DRIVE_NE]=-100;
				motor[DRIVE_NW]=-100;
				motor[DRIVE_SE]=-100;
				motor[DRIVE_SW]=-100;
			}
			halt();

			nMotorEncoder[DRIVE_NW]=0;
			while(nMotorEncoder[DRIVE_NW]<1200){
				motor[DRIVE_NE]=-100;
				motor[DRIVE_NW]=100;
				motor[DRIVE_SE]=-100;
				motor[DRIVE_SW]=100;
			}
			halt();

			ClearTimer(T1);
			nMotorEncoder[DRIVE_NW]=0;
			while(nMotorEncoder[DRIVE_NW]<5000) {//was 4500
				drive(N,100,0);
				if (time1[T1]>5000) {
					endPgm();//safety
				}
			}
		}
		else {//if bridge != none

			turnOffset=nMotorEncoder[DRIVE_NW];
			while(nMotorEncoder[DRIVE_NW]-turnOffset>-2050){
				motor[DRIVE_NE]=100;
				motor[DRIVE_NW]=-100;
				motor[DRIVE_SE]=100;
				motor[DRIVE_SW]=-100;
			}
			halt();

			turnOffset=nMotorEncoder[DRIVE_NW];
			while(nMotorEncoder[DRIVE_NW]-turnOffset>-2600){
				motor[DRIVE_NE]=-100;
				motor[DRIVE_NW]=-100;
				motor[DRIVE_SE]=-100;
				motor[DRIVE_SW]=-100;
			}
			halt();

			turnOffset=nMotorEncoder[DRIVE_NW];
			while(nMotorEncoder[DRIVE_NW]-turnOffset<1400){
				motor[DRIVE_NE]=-100;
				motor[DRIVE_NW]=100;
				motor[DRIVE_SE]=-100;
				motor[DRIVE_SW]=100;
			}
			halt();

			lineUp();

			drive(E,80,150);
			drive(N,40,300);
			if (choiceArray[2]==1) {// if bridge == far
				drive(E,80,150);
				while (onGray(csfront) && onGray(csback)) {
					drive(E,50,0);
				}
				lineUp();
				drive(E,80,400);
				while (onGray(csfront) && onGray(csback)) {
					drive(E,50,0);
				}

				lineUp();
				drive(W,80,200);
			}
			ClearTimer(T1);
			nMotorEncoder[DRIVE_NW]=0;
			while(nMotorEncoder[DRIVE_NW]<4000) {
				drive(N,100,0);
				if (time1[T1]>5000) {
					endPgm();
				}
			}
		}
	}
	halt();
    while (true) {}

}
