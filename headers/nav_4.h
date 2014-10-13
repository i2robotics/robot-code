/*--------------------------------------------------
 *
 *		Navigation v. 4
 *			utility for Mecanum wheel operation
 *			during Autonomous.
 *
 *------------------------------------------------*/

/*#define N 0							//These macros make it easy for the programmer to see
#define S 180							// which direction they are making the robot go,
#define E 90							// instead of needing to figure out an angle
#define W 270
#define NE 45
#define SE 135
#define SW 225
#define NW 315*/
#define CW 401							//Allows for spin functionality
#define AC 402
#define CCW 402						//...And a lot of duplicates for counter-clockwise
#define ACW 402
#define SPIN_RIGHT 401
#define SPIN_LEFT 402

#define N 403
#define S 404
#define E 405
#define W 406
#define NE 407
#define SE 408
#define SW 409
#define NW 410

#define FWD 562							//These macros enable the turn-like-a-car (TLAC)
#define BWD 1074						// functionality. They each set a bit beyond the range
#define RIGHT 2098					// of other uses of the direction imput, to signify
#define LEFT 4146						// the desired direction
														// They are (powers of 2) + 50, so the user can input
														// this macro + or - 50, and the resulting value, minus
														// the bit, will be a usable value from 0 to 100
#define zero 0.01

void drive(int d, byte s = 100, short t = 1000) {		//3 imputs: direction, speed, and time to wait.

	float ne =0;							//Values that will eventually become motor values.
	float se =0;
	float nw =0;
	float sw =0;

	float x = 0;							//X and Y for Trig functions
	float y = 0;

	int v = 0;							//This is needed for TLAC functionality

	if (d >= 500) {						// TLAC
		if ((512&d)==512) {v = 2*((512^d)-50);ne=s;se=s;nw=s;sw=s;
						if (v<0) {nw-=v;sw-=v;}else if (v>0) {ne-=v;se-=v;}}	//FWD
		else if ((1024&d)==1024) {v=2*((1024^d)-50);ne=-s;se=-s;nw=-s;sw=-s;
						if (v<0) {nw=+v;sw+=v;}else if (v>0) {ne+=v;se+=v;}}	//BWD
		else if ((2048&d)==2048) {v=2*((2048^d)-50);ne=-s;se=s;nw=s;sw=-s;
						if (v<0) {nw-=v;sw+=v;}else if (v>0) {ne+=v;se-=v;}}	//Right
		else if ((4096&d)==4096) {v=2*((4096^d)-50);ne=s;se=-s;nw=-s;sw=s;
						if (v<0) {nw+=v;sw-=v;}else if (v>0) {ne-=v;se+=v;}}	//Left
		else {PlayImmediateTone(200,200);wait10Msec(10);}
		/* ::EXPANDED FORM:: (for readability)
		if ((512&d)==512) {				//if the bit is set (had to use base-ten here)
			v = 2*((512^d)-50);			//Turns value from 512 to 612 into a value +/-50
			ne=s;se=s;nw=s;sw=s;		//Initally sets all values to desired speed & direction

			if (v<0) {					//If turning left
				nw-=v;sw-=v;			//Subtract from the left side
			}
			else if (v>0) {				//If turning right
				ne-=v;se-=v;			//Subtract from the right side
			}							//(or in other directions, add to a negative value)
		}
		*/
	}
	else {
		switch (d) {
		case 401: //Clockwise
			ne = 1;
			se =0;
			nw =0;
			sw = -1;
			break;
		case 402: //Counter-Clockwise / AntiClockwise
			ne =0;
			se = -1;
			nw = 1;
			sw =0;
			break;
			//
			//Old Stuffs: The trig functions below inexplicably stopped working, so I had to
							// reinstate this old code for the main cardinal directions.
			//
		case 403: //North / Forwards
			ne = 1;
			se = 1;
			nw = 1;
			sw = 1;
			break;
		case 404: //South / Backwards
			ne = -1;
			se = -1;
			nw = -1;
			sw = -1;
			break;
		case 405: //East / Right
			ne = -1;
			se = 1;
			nw = 1;
			sw = -1;
			break;
		case 406: //West / Left
			ne = 1;
			se = -1;
			nw = -1;
			sw = 1;
			break;
		case 407: //ne
			ne = -zero;
			se = 1;
			nw = 1;
			sw = -zero;
			break;
		case 408: //se
			ne = -1;
			se = zero;
			nw = zero;
			sw = -1;
			break;
		case 409: //sw
			ne = zero;
			se = -1;
			nw = -1;
			sw = zero;
			break;
		case 410: //nw
			ne = 1;
			se = zero;
			nw = zero;
			sw = 1;
			break;
			//
			//end Old Stuffs
			//
		default:
			x = cos((-d+90.0)*0.017453278);			//Trigonometry to travel at specified angle.
			y = sin((-d+90.0)*0.017453278);
			writeDebugStreamLine("x: %f, y: %f", x, y);

			ne = (y-x);
			se = (y+x);
			nw = (y+x);
			sw = (y-x);
			break;
		}
		writeDebugStreamLine(":%f,%f,%f,%f",ne,se,nw,sw);//For debuging purposes.

		ne = ne*s;
		se = se*s;
		nw = nw*s;
		sw = sw*s;
	}

	motor[DRIVE_NE] = ne;				//Finally set the motor values
	motor[DRIVE_SE] = se;
	motor[DRIVE_NW] = nw;
	motor[DRIVE_SW] = sw;

writeDebugStreamLine(":%f,%f,%f,%f",ne,se,nw,sw);//For debuging purposes.

	if (t!=0) {						//Unless time to wait is 0,
		wait1Msec(t);					// Wait that time,
		motor[DRIVE_NE] = 0;			// and then stop all motors.
		motor[DRIVE_SE] = 0;			// This way, if time is 0, motors
		motor[DRIVE_NW] = 0;			// continue indefinitely
		motor[DRIVE_SW] = 0;
	}
}

void halt() {							//Convinience function to stop all motors.
	motor[DRIVE_NE] = 0;
	motor[DRIVE_SE] = 0;
	motor[DRIVE_NW] = 0;
	motor[DRIVE_SW] = 0;
}
