/*--------------------------------------------------
 *
 *		Helpers v. 1
 *			various helpful macros
 *			and functions.
 *
 *------------------------------------------------*/

//for the joystick button bitmask
#define btn1     0x01
#define btn2     0x02
#define btn3     0x04
#define btn4     0x08
#define btn5     0x10
#define btn6     0x20
#define btn7     0x40
#define btn8     0x80
#define btn9    0x100
#define btn10   0x200
#define btn11   0x800
#define btn12  0x1000

#define bX        0x01
#define bA        0x02
#define bB        0x04
#define bY        0x08
#define bLB       0x10
#define bRB       0x20
#define bLT       0x40
#define bRT       0x80
#define bBACK    0x100
#define bSTART   0x200
#define bLSTICK  0x800
#define bRSTICK 0x1000

#define kNxtBtnRight 1
#define kNxtBtnLeft 2
#define kNxtBtnCenter 3

#define action_joy1  current_joystick = joystick.joy1_Buttons; if(false) {
#define action_joy2  current_joystick = joystick.joy2_Buttons; if(false) {
#define state }else if(current_joystick &
#define down ){
#define otherwise }else{
#define end }

#ifdef GYRO_INCLUDED
void go_to_bearing(int target)
{
	float k_p = 0.1;
	while(abs(target - bearing) > 4) {
		int error = (bearing - target) * k_p;
		writeDebugStreamLine("err: %i  bear: %i <:HelpLn53", error, bearing);
		drive_t(CW, error, 0, false);
	}
}
#endif

//for the superpro
#define LIFT_SWITCH HTSPBreadIO(HTSPB, 0x01)
#define RIGHT_GRABBER_SWITCH HTSPBreadIO(HTSPB, 0x02)
#define LEFT_GRABBER_SWITCH HTSPBreadIO(HTSPB, 0x03)
