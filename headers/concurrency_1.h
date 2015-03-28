/*--------------------------------------------------
 *
 *		Concurrency v. 1
 *			Handling of concurrent motor
 *			control during autonomous.
 *
 *------------------------------------------------*/

const unsigned int timeout_ninety = 10;
const unsigned int timeout_sixty = 9000;
const unsigned int timeout_up = 10;
const unsigned int timeout_down = 5500;

#define STOPPED 0
#define MOVING 99

#define SIXTY 1
#define NINETY 2

#define UP 1
#define DOWN 2
#define HOVER 3

typedef struct state_t
{
  byte tube;
  byte fork;
} state_t;

state_t want_state; // Public. This is the conduit for telling this system what to do.
state_t is_state; // Read-Only. This should be read by the external program to determine how to proceed.

unsigned int tube_start_time = 0; // Private
unsigned int fork_start_time = 0; // Private


void stop_tube()
{ // Private. Only to be used within this file
  writeDebugStreamLine("TUbe has been stopped");
  motor[TUBE] = 0;
  want_state.tube = STOPPED;
  tube_start_time = 0;
}

void stop_fork()
{ // Private. Only to be used within this file
  motor[FORK] = 0;
  want_state.fork = STOPPED;
  fork_start_time = 0;
}

task background_loop()
{
  while (true) {
    if (!tube_start_time && want_state.tube != STOPPED) { //if tube wants to move, and start time has not been set already
      tube_start_time = time1[T4];
    }
    if (!fork_start_time && want_state.fork != STOPPED) {
      fork_start_time = time1[T4];
    }

    switch (want_state.tube) {
      case STOPPED:
        stop_tube();
        is_state.tube = STOPPED;
        break;
      case SIXTY:
        if (NINETY_REACHED) {
          stop_tube();
          is_state.tube = NINETY;
        } else if (SIXTY_REACHED || time1[T4] - tube_start_time > timeout_sixty) {
          stop_tube();
          is_state.tube = SIXTY;
        } else {
          motor[TUBE] = 100;
          is_state.tube = MOVING;
        }
        break;
      case NINETY:
        if (NINETY_REACHED || time1[T4] - tube_start_time > timeout_ninety) {
          stop_tube();
          is_state.tube = NINETY;
        } else {
          motor[TUBE] = 100;
          is_state.tube = MOVING;
        }
        break;
    }

    switch (want_state.fork) {
      case STOPPED:
        stop_fork();
        is_state = STOPPED;
        break;
      case UP:
        if (SPATULA_UP || time1[T4] - fork_start_time > timeout_up) {
          stop_fork();
          is_state.fork = UP;
        } else {
          motor[FORK] = 100;
          is_state.fork = MOVING;
        }
        break;
      case DOWN:
        if (SPATULA_UP) {
          stop_fork();
          is_state.fork = UP;
        } else if (SPATULA_DOWN || time1[T4]-fork_start_time > timeout_down) {
          stop_fork();
          is_state.fork = DOWN;
        } else {
          motor[FORK] = -100;
          is_state.fork = MOVING;
        }
        break;
    }
    abortTimeSlice();
  }
}
