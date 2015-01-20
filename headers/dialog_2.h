/*--------------------------------------------------
 *
 *    Dialog v. 2
 *      utility for configuring autonomous
 *      using the NXT display.
 *
 *------------------------------------------------*/
#ifdef USING_CLION
#include "../headers/clion_1.h"
#endif

void dialog(Alliance_t *ret_alliance, Plan_t *ret_plan, int *ret_tubes, int *ret_delay)
{
  const string Alliance_s[] = {"Red", "Blue"};
  const string Bool_s[] = {"False", "True"};
  DEF_PLAN_STRINGS

  int sel_option = 0;
  typedef struct
  {
    Alliance_t alliance;
    Plan_t plan;
    int tubes;
    int delay;
  } Options_t;
  //set default options
  Options_t options;
  options.alliance = (Alliance_t) 0;
  options.plan = (Plan_t) 0;
  options.tubes = 0;
  options.delay = 0;

  bool ready = false;
  ClearTimer(T1);
  while (!ready) {
    eraseDisplay();
    nxtDisplayStringAt(15, 62, "Alli.:");
    nxtDisplayStringAt(15, 52, "Plan :");
    nxtDisplayStringAt(15, 42, "Tubes:");
    nxtDisplayStringAt(15, 32, "Delay:");
    nxtDrawLine(0, 22, 100, 22);
    nxtDisplayStringAt(5, 18, "Ext Batt: %.2f", ((float) externalBatteryAvg) / 1000);
    nxtDisplayStringAt(5, 8, "NXT Batt: %.2f", ((float) nAvgBatteryLevel) / 1000);


    nxtDisplayStringAt(55, 62, "%s", Alliance_s[options.alliance]);
    nxtDisplayStringAt(55, 52, "%s", Plan_s[options.plan]);
    nxtDisplayStringAt(55, 42, "%s", Bool_s[options.tubes]);
    nxtDisplayStringAt(55, 32, "%i", options.delay);
    nxtDisplayStringAt(5, 62 - sel_option * 10, ">");

    switch (nNxtButtonPressed) {
      case kNxtBtnRight:
        if (time10[T1] > 30) {
          switch (sel_option) {
            case 0: // Alliance
              options.alliance = options.alliance ? 0 : 1;
              //                 if it != 0 (true) -^ | ^- if it == 0 (false)
              // Here I'm leveraging the fact that an integer with value 0 evalues to false in C.
              break;
            case 1: // Plan
              options.plan = options.plan ? options.plan - 1 : 1; //<-- Max (we'll have more eventually)
              // if plan > 0, decrement.      If it's 0, set it back to the max value
              break;
            case 2: // Tubes
                options.tubes = options.tubes ? 0 : 1;
              break;
            case 3: // Delay
              if (options.delay < 20) { // Can't use a special trick here
                options.delay += 5;
              } else {
                options.delay = 0;
              }
              break;
            default:
              sel_option = 0;
              break;
          }
          ClearTimer(T1);
        }
        break;
      case kNxtBtnLeft:
        if (time10[T1] > 30) {
          if (sel_option == 3) {
            sel_option = 0;
          } else {
            sel_option++;
          }
          ClearTimer(T1);
        }
        break;
      case kNxtBtnCenter:
        ready = true;
        break;
    }
    wait1Msec(20);
  }

  *ret_plan = options.plan;
  *ret_alliance = options.alliance;
  *ret_tubes = options.tubes;
  *ret_delay = options.delay;
  return;
}
