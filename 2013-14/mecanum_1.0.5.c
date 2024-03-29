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

int NEval;
int NWval;
int SEval;
int SWval;
int X2 = 0, Y1 = 0, X1 = 0, threshold = 5;
float angle;
task main()
{
	while (true) {
		getJoystickSettings(joystick);

		//Create "deadzone" for Y1/Ch3
		if(scaleJoy(joystick.joy1_x1) > threshold && scaleJoy(joystick.joy1_y1) > threshold) {
		angle = atan((float)joystick.joy1_y1/(float)joystick.joy1_x1);
			NEval = 100.0*sin((float)angle-0.7853981)/cos((float)angle-0.7853981);
			SWval = 100.0*sin((float)angle-0.7853981)/cos((float)angle-0.7853981);
			SEval = 100.0;
			NWval = 100.0;
		}
		if(-scaleJoy(joystick.joy1_x1) > threshold && -scaleJoy(joystick.joy1_y1) > threshold) {
		angle = atan((float)joystick.joy1_y1/(float)joystick.joy1_x1);
			NEval = 100.0*sin((float)angle-0.7853981)/cos((float)angle-0.7853981);
			SWval = 100.0*sin((float)angle-0.7853981)/cos((float)angle-0.7853981);
			SEval = 100.0;
			NWval = 100.0;
		}
		if(-scaleJoy(joystick.joy1_x1) > threshold && scaleJoy(joystick.joy1_y1) > threshold) {
		angle = atan((float)joystick.joy1_y1/(float)joystick.joy1_x1);
			SEval = 100.0*sin((float)angle-0.7853981)/cos((float)angle-0.7853981);
			NWval = 100.0*sin((float)angle-0.7853981)/cos((float)angle-0.7853981);
			NEval = 100.0;
			SWval = 100.0;
		}
		if(scaleJoy(joystick.joy1_x1) > threshold && -scaleJoy(joystick.joy1_y1) > threshold) {
		angle = atan((float)joystick.joy1_y1/(float)joystick.joy1_x1);
			SEval = 100.0*sin((float)angle-0.7853981)/cos((float)angle-0.7853981);
			NWval = 100.0*sin((float)angle-0.7853981)/cos((float)angle-0.7853981);
			NEval = 100.0;
			SWval = 100.0;
		}
		else {
			SEval = 0;
			NWval = 0;
			NEval = 0;
			SWval = 0;
	}
		//Create "deadzone" for X2/Ch1

		//Remote Control Commands
		motor[DRIVE_NE] = NEval;
		motor[DRIVE_SE] = SEval;
		motor[DRIVE_NW] = NWval;
		motor[DRIVE_SW] = SWval;

	}


}
