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
int X2 = 0, Y1 = 0, X1 = 0, threshold = 15;
task main()
{
	while (true) {
		getJoystickSettings(joystick);

		//Create "deadzone" for Y1/Ch3
		if(abs(scaleJoy(joystick.joy1_y1)) > threshold)
			Y1 = scaleJoy(joystick.joy1_y1);
		else
			Y1 = 0;
		//Create "deadzone" for X1/Ch4
		if(abs(scaleJoy(joystick.joy1_x1)) > threshold)
			X1 = scaleJoy(joystick.joy1_x1);
		else
			X1 = 0;
		//Create "deadzone" for X2/Ch1
		if(abs(scaleJoy(joystick.joy1_x2)) > threshold)
			X2 = scaleJoy(joystick.joy1_x2);
		else
			X2 = 0;

		//Remote Control Commands
		motor[DRIVE_NE] = Y1 - X2 - X1;
		motor[DRIVE_SE] =  Y1 - X2 + X1;
		motor[DRIVE_NW] = Y1 + X2 + X1;
		motor[DRIVE_SW] =  Y1 + X2 - X1;
	}


}
