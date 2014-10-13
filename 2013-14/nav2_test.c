int DRIVE_NE;
int DRIVE_SE;
int DRIVE_NW;
int DRIVE_SW;

#include "./nav_2.00.h"

task main()
{
	//drive(0);
	drive(FWD+15);
	drive(FWD-30);
	drive(BWD+12);
	drive(FWD+12);
	drive(N);
	drive(12);
	drive(FWD);
	drive(AC);

}
