#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "./scaleJoy.h" //Our own code to scale joystick values.

task main()
{
	while (true)
	{
		getJoystickSettings(joystick);


		if (joystick.joy1_Buttons & button2){ //A
			PlayImmediateTone(200,1);
		}

	}
}
