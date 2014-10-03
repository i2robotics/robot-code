#define FWD 1
#define BWD 3
#define RIGHT 2
#define LEFT 4
#define SPIN_RIGHT 9
#define SPIN_LEFT 10

#define N 1
#define S 3
#define E 2
#define W 4
#define NE 5
#define SE 6
#define SW 7
#define NW 8
#define CW 9
#define AC 10
#define CCW 10
#define ACW 10

void drive(int d, int s = 100, short t = 1000) {
	
	int ne =0;
	int se =0;
	int nw =0;
	int sw =0;
	
	switch (d) {
	case 1: //North / Forwards
		ne = 1;
		se = 1;
		nw = 1;
		sw = 1;
	break;
	case 2: //East / Right
		ne = -1;
		se = 1;
		nw = 1;
		sw = -1;
	break;
	case 3: //South / Backwards
		ne = -1;
		se = -1;
		nw = -1;
		sw = -1;
	break;
	case 4: //West / Left
		ne = 1;
		se = -1;
		nw = -1;
		sw = 1;
	break;
	case 5: //ne
		ne =0;
		se = 1;
		nw = 1;
		sw =0;
	break;
	case 6: //se
		ne = 1;
		se = 1;
		nw = 1;
		sw = 1;
	break;
	case 7: //sw
		ne =0;
		se = -1;
		nw = -1;
		sw =0;
	break;
	case 8: //nw
		ne = 1;
		se =0;
		nw =0;
		sw = 1;
	break;
	case 9: //Clockwise
		ne = 1;
		se =0;
		nw =0;
		sw = -1;
	break;
	case 10: //Counter-Clockwise / AntiClockwise
		ne =0;
		se = -1;
		nw = 1;
		sw =0;
	break;
	}
	
	motor[DRIVE_NE] = ne*s;
	motor[DRIVE_SE] = se*s;
	motor[DRIVE_NW] = nw*s;
	motor[DRIVE_SW] = sw*s;
	
	if (t!=0) {
		Wait1Msec(t);
		motor[DRIVE_NE] = 0;
		motor[DRIVE_SE] = 0;
		motor[DRIVE_NW] = 0;
		motor[DRIVE_SW] = 0;
	}
}
