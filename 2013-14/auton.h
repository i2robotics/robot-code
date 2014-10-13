#define C 1234
#define CC 123

int X1;
int Y1;

void drive(float degree = 0, int speed = 100, int time= 0) {
	X1 = speed*cos(-degree+90.0);
	Y1 = speed*sin(-degree+90.0);

	motor[DRIVE_NE] = Y1-X1;
	motor[DRIVE_SE] = Y1+X1;
	motor[DRIVE_NW] = Y1+X1;
	motor[DRIVE_SW] = Y1-X1;

	wait1Msec(time);

	if (time != 0){
		motor[DRIVE_NE] = 0;
		motor[DRIVE_SE] = 0;
		motor[DRIVE_NW] = 0;
		motor[DRIVE_SW] = 0;
	}
}

void halt() {
	motor[DRIVE_NE] = 0;
	motor[DRIVE_SE] = 0;
	motor[DRIVE_NW] = 0;
	motor[DRIVE_SW] = 0;
}

void turn(float degrees,int direction = 123) {
	if (direction!=123 && direction!=1234) {
		direction = 123;
	}
	if (direction == 123) {
		motor[DRIVE_NE] = 60;
		motor[DRIVE_SE] = 60;
		motor[DRIVE_NW] = -60;
		motor[DRIVE_SW] = -60;
	}
	else {
		motor[DRIVE_NE] = -60;
		motor[DRIVE_SE] = -60;
		motor[DRIVE_NW] = 60;
		motor[DRIVE_SW] = 60;
	}
	wait10Msec(degrees);
	motor[DRIVE_NE] = 0;
	motor[DRIVE_SE] = 0;
	motor[DRIVE_NW] = 0;
	motor[DRIVE_SW] = 0;
}
