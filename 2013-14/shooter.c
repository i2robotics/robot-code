#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     btn,            sensorTouch)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{

		//getJoystickSettings(joystick);
			motor[motorD]=100;
			motor[motorE]=100;

		/*else if (nNxtButtonPressed == 0 || nNxtButtonPressed == 2) {
			motor[motorD]=100;
			motor[motorE]=100;*/
		//}

Sleep(10000000);
}
