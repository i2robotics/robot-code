#define FRONT 242
#define RIGHTSIDE 121
#define BACK 0

int counter=0;
float percentSeen = 0;

void moveServo(int pos = 242) {
	servo[ULTRA_ROT] = pos;
	if (ServoValue[ULTRA_ROT] < 60) {
		wait1Msec(700);
	}
	else if (ServoValue[ULTRA_ROT]<180) {
		wait1Msec(500);
	}
	else {
		wait1Msec(250);
	}
	wait1Msec(1000);
}

bool check(int distance = 60,int cap = 50) {
	if (SensorValue[ULTRASONIC]<distance) {
		counter++;
	}
	else {
		if (counter>0) {
			counter--;
		}
	}
	if (counter>cap) {
		counter = 0;
		return true;
	}
	return false;
}

void resetCounter() {
	counter = 0;
}

bool checkNow(int distance = 60, int cap = 50) {
	percentSeen = 0.0;
	counter = 0;
	int counter2 = 0;
	while (counter<cap) {
		if (SensorValue[ULTRASONIC]<distance) {
			counter2++;
		}
		counter++;
	}
	percentSeen = (float)counter2/(float)cap;
	if (percentSeen>0.5) {
		return true;
	}
	return false;
}
