/*--------------------------------------------------
*
*   Lift Control v. 1
*     Measuring and altering height
*
*------------------------------------------------*/
int lift_height = 0;
task lift_control () {
	HTSPBsetupIO(HTSPB, 0x10);
	while(true) {
		if(HTSPBreadIO(HTSPB, 0x01) != 1) {
      nxtDisplayTextLine(1, "Magnet absent");
      //HTSPBwriteIO(HTSPB, 0x10);
    } else {
      nxtDisplayTextLine(1, "Magnet present");
      //HTSPBwriteIO(HTSPB, 0x00);
    }
		nxtDisplayTextLine(2, "lift: %i", lift_height);
		while (HTSPBreadIO(HTSPB, 0x01) != 1) {};
		//nxtDisplayTextLine(2, "hi");
		if (joystick.joy1_y2 > 5) {
			lift_height++;
		} else {
			lift_height--;
		}

		while (HTSPBreadIO(HTSPB, 0x01) == 1) {};
		if (joystick.joy1_y2 > 5) {
			lift_height++;
		} else {
			lift_height--;
		}
		wait1Msec(50);
	}
}
