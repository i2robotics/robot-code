int stuff = 0;
short encoderVal;
short coolVal=0;
int coolVal2;
bool sawFront = false;
bool sawFrontSide = false;
bool sawFrontSide2 = false;
int ultraValue = 0;
int ultraValue2 = 0;
int ultraValue3 = 0;
int speed = 50;
bool codeCheck1 = false;
bool codeCheck2 = false;
bool codeCheck3 = false;
bool codeCheck4 = false;

void zig1F() {
	motor[DRIVE_NW] = 90;
	motor[DRIVE_SE] = 90;
	motor[DRIVE_NE] = -22;
	motor[DRIVE_SW] = -22;
	wait1Msec(1750);
	halt();
}

void zig2F() {
	motor[DRIVE_NW] = 40;
	motor[DRIVE_SE] = 40;
	motor[DRIVE_NE] = -90;
	motor[DRIVE_SW] = -90;
	wait1Msec(1400);
	nMotorEncoder[DRIVE_NW]=0;
	while(nMotorEncoder[DRIVE_NW]>-4900) {
		drive(S,100,0);
	}
}

void zig1FB() {
	motor[DRIVE_NW] = -90;
	motor[DRIVE_SE] = -90;
	motor[DRIVE_NE] = 22;
	motor[DRIVE_SW] = 22;
	wait1Msec(1750);
	halt();
}

void zig1B() {
	motor[DRIVE_NW] = 22;
	motor[DRIVE_SE] = 22;
	motor[DRIVE_NE] = -90;
	motor[DRIVE_SW] = -90;
	wait1Msec(1750);
	motor[DRIVE_NW] = 90;
	motor[DRIVE_SE] = 90;
	motor[DRIVE_NE] = -44;
	motor[DRIVE_SW] = -44;
	wait1Msec(1400);
	nMotorEncoder[DRIVE_NW]=0;
	while(nMotorEncoder[DRIVE_NW]<4900) {
		drive(N,100,0);
	}
}

short driveBack(short coolVal) {
	while (coolVal>1000) {
		drive(S,speed,0);
		coolVal = encoderVal + nMotorEncoder[DRIVE_NW];
	}
	halt();
	return coolVal;
}

short driveForwardB(short coolVal) {
	while (coolVal<-1000) {
		drive(N,speed,0);
		coolVal = encoderVal + nMotorEncoder[DRIVE_NW];
	}
	halt();
	return coolVal;
}

short driveForwardC(short coolVal) {
	while (coolVal<7000) {
		codeCheck2 = true;
		drive(N,speed,0);
		coolVal = encoderVal + nMotorEncoder[DRIVE_NW];
	}
	halt();
	return coolVal;
}

void adjustF() {
	nMotorEncoder[DRIVE_NW] = 0;
	while (nMotorEncoder[DRIVE_NW]<2350) {
		drive(N,speed,0);
	}
	nMotorEncoder[DRIVE_NW] = 0;
	while (nMotorEncoder[DRIVE_NW]>-2100) {
		drive(W,speed,0);
	}
}
