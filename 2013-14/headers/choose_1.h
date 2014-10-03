/*--------------------------------------------------
 *
 *		Choose v. 1
 *			utility for choosing Autonomous program
 *			options.
 *
 *------------------------------------------------*/

int choiceArray[] = {1, 1, 2}; //
int startDelay = 3;

void choose_auto_opts()
{
	int selected = 0;
	ClearTimer(T2);
	const char *START_CHOICES[] = {"Left","Right"}; // Starting position on the field.
	const char *END_CHOICES[] = {"Left","Right"}; // Direction to go once block has dumped.
	const char *BRIDGE_CHOICES[] = {"Close","Far","No"}; // Which line to align to, or to not align.

	while (true) {
		eraseDisplay();
		nxtDisplayStringAt(15, 62, "Start:");
		nxtDisplayStringAt(15, 52, "End:");
		nxtDisplayStringAt(15, 42, "Bridge:");
		nxtDisplayStringAt(15, 32, "Delay:");
		nxtDrawLine (0, 22, 100, 22);
		nxtDisplayStringAt(5, 18, "Ext Batt: %.2f", ((float) externalBatteryAvg)/1000);
		nxtDisplayStringAt(5, 8, "NXT Batt: %.2f", ((float) nAvgBatteryLevel)/1000);

		nxtDisplayStringAt(55, 62, "%s", START_CHOICES[choiceArray[0]]);
		nxtDisplayStringAt(43, 52, "%s", END_CHOICES[choiceArray[1]]);
		nxtDisplayStringAt(59, 42, "%s", BRIDGE_CHOICES[choiceArray[2]]);
		nxtDisplayStringAt(55, 32, "%d", startDelay);
		nxtDisplayStringAt(5, 62 - selected*10, "*");

		if (nNxtButtonPressed == 1) { // button press right
			if (time10[T2]>30) {
				switch (selected)
				{
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
					if (startDelay < 20){
						startDelay++;
					}
					else {
						startDelay = 0;
					}
					break;
				}
				ClearTimer(T2);
			}
		}
		if (nNxtButtonPressed == 2) {// on button press Left
			if (time10[T2]>30) {
				if (selected == 3) {
					selected = 0;
				}
				else {
					selected++;             //increment selected
				}
				ClearTimer(T2);
			}
		}
		if (nNxtButtonPressed == 3) { // button orange
			break;
		}
		wait1Msec(20);
	}
}
