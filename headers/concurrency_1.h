/*--------------------------------------------------
 *
 *		Concurrency v. 1
 *			Handling of concurrent motor
 *			control during autonomous.
 *
 *------------------------------------------------*/

const unsigned int timeout_ninety = 5000;
const unsigned int timeout_sixty = 9000;
const unsigned int timeout_up = 10;
const unsigned int timeout_down = 5700;

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
  motor[TUBE] = 0;
  //want_state.tube = STOPPED; //problem with this:
  tube_start_time = 0;
}

void stop_fork()
{ // Private. Only to be used within this file
  motor[FORK] = 0;
  want_state.fork = STOPPED; //problem without this: if it times out, it'll keep going because It'll reset the timer, and then start the thing going again
  fork_start_time = 0;
}

bool check_tout(unsigned int limit_time, unsigned int check_time) {
  if (time1[T4] - check_time > limit_time) {
    writeDebugStreamLine("Timed Out! :%i", limit_time);
    return true;
  } else {
    return false;
  }
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
        } else if (SIXTY_REACHED || check_tout(timeout_sixty, tube_start_time)) {
          stop_tube();
          is_state.tube = SIXTY;
        } else {
          motor[TUBE] = 100;
          is_state.tube = MOVING;
        }
        break;
      case NINETY:
        if (NINETY_REACHED || check_tout(timeout_ninety, tube_start_time)) {
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
        is_state.fork = STOPPED;
        break;
      case UP:
        if (SPATULA_UP || check_tout(timeout_up, fork_start_time)) {
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
        } else if (SPATULA_DOWN || check_tout(timeout_down, fork_start_time)) {
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
