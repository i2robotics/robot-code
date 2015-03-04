#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
#include "../drivers/hitechnic-sensormux.h"
#include "../drivers/hitechnic-irseeker-v2.h"
#include "../drivers/lego-touch.h"
#include "../drivers/hitechnic-superpro.h"
#include "../drivers/lego-ultrasound.h"

const tMUXSensor LEGOUS = msensor_S4_2;

task main () {
  int dist = 0;

  nxtDisplayCenteredTextLine(0, "Lego");
  nxtDisplayCenteredBigTextLine(1, "US");
  nxtDisplayCenteredTextLine(3, "SMUX Test");
  nxtDisplayCenteredTextLine(5, "Connect SMUX to");
  nxtDisplayCenteredTextLine(6, "S1 and US sensor");
  nxtDisplayCenteredTextLine(7, "to SMUX Port 1");
  wait1Msec(2000);

  eraseDisplay();
  nxtDisplayTextLine(0, "Lego US Sensor");

  while(true) {
    // Read the current distance detected.
    dist = USreadDist(LEGOUS);

    // display the info from the sensor
    nxtDisplayTextLine(3, "Dist:  %3d cm", dist);
    wait10Msec(50);
  }
}
