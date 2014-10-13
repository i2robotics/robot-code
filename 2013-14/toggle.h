short buttonTimeArray[12] = { nPgmTime, nPgmTime, nPgmTime, nPgmTime, nPgmTime, nPgmTime, nPgmTime, nPgmTime, nPgmTime, nPgmTime, nPgmTime, nPgmTime};
int buttonBoolArray[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool toggle(int joystick = 1, int button) {
	if (nPgmTime - buttonTimeArray[button-1] > 300) {
		if (buttonBoolArray[button-1]==1) {buttonBoolArray[button-1] = 0;}
		else {buttonBoolArray[button-1] = 1;}
		buttonTimeArray[button-1] = nPgmTime;
	}
	if (buttonBoolArray[button-1]==1) {return true;}
	return false;
}
