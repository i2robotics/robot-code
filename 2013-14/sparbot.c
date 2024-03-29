#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,          flag,          tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          flag2,         tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     left,          tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     right,         tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    Rotate,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S2_C1_2,    locking,              tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "./scalejoy.h"

unsigned short button;
float Y1;
float X2;

task main()
{


	while(true){
		if(joystick.joy1_Buttons == 32){
			motor[right] = 100;
		motor[left]= 100;
		wait10Msec(250);
		}
		Y1 = scaleJoy(joystick.joy1_y1);
		X2 = scaleJoy(joystick.joy1_x1);
		motor[right] = -1.0*(Y1-X2);
		motor[left]= -1.0*(Y1+X2);
		button = joystick.joy1_Buttons;
		if(joystick.joy1_Buttons == 128){
			motor[flag] = 100;
			motor[flag2] = 100;
	}else{
		motor[flag] = 0;
		motor[flag2] = 0;
		}
if(scaleJoy(joystick.joy1_x2) < 0){
	servo[locking] = 0;
}
if (scaleJoy(joystick.joy1_x2) < 0) {
	servo[Rotate]=0;
}
else if (scaleJoy(joystick.joy1_x2) > 0) {
	servo[Rotate]= 255;
}
else {
	servo[Rotate] = 127;
}

//servo[Rotate] = scaleJoy(joystick.joy1_x2);
if(joystick.joy1_Buttons == 16){
	servo[locking] = 255;
}
else {
	servo[locking] = 0;
}
}
}
