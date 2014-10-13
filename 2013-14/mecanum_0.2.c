#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     DRIVE_SE,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     DRIVE_SW,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     DRIVE_NW,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     DRIVE_NE,      tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

#define button1	   0x01
#define button2	   0x02
#define button3	   0x04
#define button4	   0x08
#define button5	   0x10
#define button6	   0x20
#define button7	   0x40
#define button8	   0x80
#define button9	  0x100
#define button10  0x200
#define button11  0x800
#define button12 0x1000



const bool bLogarithmicScale = true;
const bool kMaximumPowerLevel = 100;  // Adjust to set max power level to be used.

int scaleJoy(int &nJoy1, int nMaxValue = kMaximumPowerLevel)
{
	//
	// This function scales the joystick settings to the appropriate range for
	// controlling a NXT motor.
	//
	// Joystick values range from -128 to +127.
	// Speed/power settings for NXT motors range from -100 to +100
	//
	// The physical range of motion of a joystick is quite small and it is sometimes
	// hard to control slow speeds. So another capability of this program to apply
	// a "logarithmic" scale to the joystick settings.
	//
	static const int nLogScale[17] =
	{
		0,	 5,	  9,   10,
		12,	 15,  18,  24,
		30,	 36,  43,  50,
		60,	 72,  85, 100,
		100
	};
	int nScaled;

	nScaled = nJoy1;
	if (bLogarithmicScale)
	{
		nScaled /= 8;
		if (nScaled >= 0)
			nScaled = nLogScale[nScaled];
		else
			nScaled = - nLogScale[ - nScaled];
	}
	nScaled *= nMaxValue;
	nScaled /= 100;
	return nScaled;
}

int NEval=0;
int NWval=0;
int SEval=0;
int SWval=0;

task main()
{
	while (true) {
		getJoystickSettings(joystick);

		if(abs(joystick.joy1_y1)>abs(joystick.joy1_x1)+10) {
			NEval = scaleJoy(joystick.joy1_y1);
			NWval = scaleJoy(joystick.joy1_y1);
			SEval = scaleJoy(joystick.joy1_y1);
			SWval = scaleJoy(joystick.joy1_y1);
		}
		else if(abs(joystick.joy1_x1)>abs(joystick.joy1_y1)+10) {
			NEval= (scaleJoy(joystick.joy1_x1));
			NWval= -(scaleJoy(joystick.joy1_x1));
			SEval= -(scaleJoy(joystick.joy1_x1));
			SWval= (scaleJoy(joystick.joy1_x1));
		}
		else{
				NEval= 0;
				NWval= -(scaleJoy(joystick.joy1_x2));
				SEval= (scaleJoy(joystick.joy1_x2));
				SWval= 0;
			}
			//else {
			//	NEval = 0;																				//this will eliminate the
			//	SEval = 0;																				//joystick being partially
			//	NWval = 0;																				//off center, and powering
			//	//problem caused by the
			//	SWval = 0;																				//the motors when not intended
			//}

		motor[DRIVE_NE]=NEval;
		motor[DRIVE_NW]=NWval;
		motor[DRIVE_SE]=SEval;
		motor[DRIVE_SW]=SWval;

	}
}
