#include <avr/io.h>
#include <stdio.h>
#include "m_general.h"
#include "timer.h"
#include "comm.h"
#include "camera.h"
#include "debug.h"
#include "motor.h"
#include "adc.h"
#include "control.h"
#include "features.h"
#include "puck.h"
#include "constants.h"
#include "goalie.h"

volatile int camera_timer_flag = 0;
volatile int new_packet_flag = 0;
POINT *robot = NULL;
volatile int mrf_count = 0;

void m2_init();

void init_robot_and_features() {
  // ROBOT_NAME DETERMINES ALL ROBOT SPECIFIC CONSTANTS.
  /* ROBOT_NAME = UGLY; */
  ROBOT_NAME = HOPE;
  /* ROBOT_NAME = GENERAL; */
  init_constants(ROBOT_NAME);

  // FLAGS DECIDE ALL BEHAVIOR.
  LOCALIZATION = 1;
  FIND_PUCK = 1;
  FIND_GOAL = 1;
  STOP_OWN_GOAL = 1;
  REQUIRE_COMM = 1;
  FOLLOW_WALL = 0;

  TRACK_PUCK = 0; //ONLY FOR GOALIE.

  TEST_SENSORS = 0;
  TEST_GO_FORWARD = 0;
  TEST_GO_BACKWARD = 0;
  TEST_LOCALIZATION_CENTER = 0;
  TEST_STAR_READING = 0;
  TEST_HAVE_PUCK = 1;
  TEST_PUCK_SENSOR = 0;
  TEST_WALL_TROUBLE = 0;
}

int main(void) {
  m2_init();

  while(1) {
    if (TEST_HAVE_PUCK) {
      light_up_with_puck();
    }

    if (new_packet_flag) {
      comm_handler();
      new_packet_flag = 0;
    }

    if (camera_timer_flag) {
      robot = localize_robot();
      camera_timer_flag = 0;
    }

    set_goal();

    if (!is_play()) {
      stop();
      continue;
    }

    update_ADC();

    if (TRACK_PUCK) {
      track_puck();
      continue;
    }

    if (!have_puck()) {
      find_puck(robot);
    }

    if (STOP_OWN_GOAL && headed_own_goal(robot)) {
      stop();
    } else {
      if (FOLLOW_WALL && near_wall(robot) && have_puck()) {
        find_puck(robot);
        continue;
      }
      if (!FIND_PUCK || have_puck()) {
        drive_to_goal(robot);
      }
    }
  }
  return 0;
}

void m2_init() {
  // initialize robot constants and feature flags.
  init_robot_and_features();

  // set system clock to 16MHz.
  m_clockdivide(0);

  // enable global interrupts.
  sei();

  // enable switch input pins.
  clear(DDRB, 0);
  clear(DDRB, 1);

  // enable Bi-color LED output pins.
  /* clear(DDRB, 2); */
  /* clear(DDRB, 3); */

  // usb data channel init.
  debug_init();

  // init motor pins.
  motor_init();

  // init adc
  init_ADC();

  // motor enable pin pwm timer.
  init_timer1();

  // camera polling timer.
  init_timer3();

  // mWii init.
  camera_init();

  // rf communicaiton init.
  comm_init();

  if (TEST_GO_FORWARD) {
    set_motor_duty_cycle(200, 200);
  } else if (TEST_GO_BACKWARD) {
    set_motor_duty_cycle(-200, -200);
  }
}

// camera interrupt.
ISR(TIMER3_COMPA_vect) {
  camera_timer_flag = 1;
  mrf_count++;
  if (mrf_count % 10 == 0) {
    comm_reopen();
  }
}

//rf data interrupt.
ISR(INT2_vect){
  new_packet_flag = 1;
}

//adc interrupt
ISR(ADC_vect)
{  // this interrupt is present to clear ADCSRA: ADIF flag (in this
    // case write a logivcal one to it)
}

