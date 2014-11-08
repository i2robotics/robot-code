/*--------------------------------------------------
 *
 *		Dialog v. 2
 *			utility for configuring autonomous
 *			using the NXT display.
 *
 *------------------------------------------------*/

void dialog(Plan_t *plan, Alliance_t *alliance, )
{
  int selected = 0;
  ClearTimer(T1);
  const char *startChoices[] = {"Left","Right"};
  const char *endChoices[] = {"Left","Right"};
  const char *bridgeChoices[] = {"Close","Far","No"};
  while (true) {
    eraseDisplay();
    nxtDisplayStringAt(15, 62, "Start:");
    nxtDisplayStringAt(15, 52, "End:");
    nxtDisplayStringAt(15, 42, "Bridge:");
    nxtDisplayStringAt(15, 32, "Delay:");
    nxtDrawLine (0, 22, 100, 22);
    nxtDisplayStringAt(5, 18, "Ext Batt: %.2f", ((float) externalBatteryAvg)/1000);
    nxtDisplayStringAt(5, 8, "NXT Batt: %.2f", ((float) nAvgBatteryLevel)/1000);

    nxtDisplayStringAt(55, 62, "%s", startChoices[choiceArray[0]]);
    nxtDisplayStringAt(43, 52, "%s", endChoices[choiceArray[1]]);
    nxtDisplayStringAt(59, 42, "%s", bridgeChoices[choiceArray[2]]);
    nxtDisplayStringAt(55, 32, "%d", delay);
    //sizeof(startChoices) / sizeof(startChoices[0]);
    nxtDisplayStringAt(5, 62 -selected*10, "*");

    if (nNxtButtonPressed == 1) {//button press right
      if (time10[T1]>30) {
        switch (selected) {
        case 0:
          if (choiceArray[0]==1) {
            choiceArray[0] = 0;
          }
          else {
            choiceArray[0]++;
          }
          break;
        case 1:
          if (choiceArray[1]==1) {
            choiceArray[1] = 0;
          }
          else {
            choiceArray[1]++;
          }
          break;
        case 2:
          if (choiceArray[2]==2) {
            choiceArray[2] = 0;
          }
          else {
            choiceArray[2]++;
          }
          break;
        case 3:
          if (delay < 20){
            delay++;
          }
          else {
            delay = 0;
          }
          break;
        }
        ClearTimer(T1);
      }
    }
    if (nNxtButtonPressed == 2) {// on button press Left
      if (time10[T1]>30) {
        if (selected == 3) {
          selected = 0;
        }
        else {
          selected++;             //increment selected
        }
        ClearTimer(T1);
      }
    }
    if (nNxtButtonPressed == 3) { // button orange
      break;
    }


    wait1Msec(20);
  }
  return;
}
