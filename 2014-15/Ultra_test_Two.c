#pragma config(Sensor, S1,     one,            sensorSONAR)
#pragma config(Sensor, S2,     two,            sensorSONAR)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
int ult1;
int ult2;
int position;
task main()
{
  clearDebugStream(); // 40>x<70 is 3
  //70<x>90 is 1
  // 2 is 255
  while(true) {
    wait1Msec(500);
    ult1 = SensorValue(S1);
    ult2 = SensorValue(S2);
    if (ult1 > 70 && ult1 < 90 && (ult1 - ult2) < 15) {
      position = 1;
    } else if (ult1 > 40 && ult1 < 60 && (ult1 - ult2) < 15) {
      position = 3;
    } else if (ult1 > 200 && (ult1 - ult2) < 15) {
      position = 2;
    }
    nxtDisplayTextLine(1, "Sensor 1: %i", ult1);
    nxtDisplayTextLine(2, "Sensor 2: %i", ult2);
    nxtDisplayTextLine(3, "Difference: %i", ult1 - ult2);
    nxtDisplayBigTextLine(4, "Pos: %i", position);
    writeDebugStreamLine("Sensor 1: %i", ult1);
    writeDebugStreamLine("Sensor 2: %i", ult2);
    writeDebugStreamLine("Difference: %i", ult1 - ult2);
  }
}