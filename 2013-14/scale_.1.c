

int scaleJoy(int &nJoy1)
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

	int nScaled;

	nScaled = nJoy1;
	if nScaled>10 {
		if nScaled>115 {
			return 100;
		}
		else {
			return (1.04^nScaled);
		}
	}
	else if nScale<-10{
		if nScaled<-115 {
			return -100;
		}
		else {
			return -(1.04^nScaled)
		}
	}
	else {
		return 0;
	}
}
