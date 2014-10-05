/*--------------------------------------------------
 *
 *		Helpers v. 1
 *			various helpful macros
 *			and functions.
 *
 *------------------------------------------------*/

//for the joystick button bitmask
#define btn1	   0x01
#define btn2	   0x02
#define btn3	   0x04
#define btn4	   0x08
#define btn5	   0x10
#define btn6	   0x20
#define btn7	   0x40
#define btn8	   0x80
#define btn9	  0x100
#define btn10   0x200
#define btn11   0x800
#define btn12  0x1000

//Modification of a switch statement, which does a bitwise and
#define BIT_SWITCH(x) for (uint16_t bit = 1; x >= bit; bit *= 2) if (x & bit) switch (bit)
