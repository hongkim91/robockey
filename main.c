#include <avr/io.h>
#include <stdio.h>
#include "m_general.h"
#include "timer.h"
#include "comm.h"
#include "camera.h"
#include "debug.h"
#include "motor.h"
#include "localization.h"

#define RXADDRESS 84

volatile int new_camera_data_flag = 0;
volatile int new_packet_flag = 0;
unsigned int blobs[12];

float x;
float y;
float robot_theta;
float goal_theta;
POINT *robot_goal[2];

int main(void) {
  // set system clock to 16MHz.
  m_clockdivide(0);

  // enable global interrupts.
  sei();

  // usb data channel init.
  debug_init();

  // init motor pins.
  motor_init();

  // motor enable pin pwm timer.
  /* init_timer1(); */

  // camera polling timer.
  init_timer3();

  // mWii init.
  camera_init();

  // rf communicaiton init.
  comm_init(RXADDRESS);

  robot_goal[1] = create_point(820, 350);

  /* rotate(CLOCKWISE); */
  /* go(); */
  /* stop(); */
  /* /\* go(); *\/ */

  while(1) {
    if (new_camera_data_flag) {
      /* m_red(TOGGLE); */
      camera_handler(blobs, &x, &y, &robot_theta);
      if (x > 800) {
        stop();
        continue;
      }

      robot_goal[0] = create_point(x,y);
      goal_theta = determine_angle(robot_goal);
      if (fabs(robot_theta - goal_theta) < 0.20) {
        m_red(ON);
        m_green(OFF);
        /* stop(); */
        go_forward();
      } else {
        m_red(OFF);
        m_green(ON);
        rotate(CLOCKWISE);
        go();
      }

      /* send_camera_data(blobs, robot_theta*57.3, goal_theta*57.3); */

      send_float("robot_theta", robot_theta * 57.3);
      send_float("goal_theta", goal_theta * 57.3);
      free(robot_goal[0]);
      /* send_camera_data(blobs, x, y); */
      new_camera_data_flag = 0;
    }
    if (new_packet_flag) {
      m_green(TOGGLE);
      comm_handler();
      new_packet_flag = 0;
    }
  }
  return 0;
}

// camera interrupt.
ISR(TIMER3_COMPA_vect) {
  new_camera_data_flag = 1;
}

// rf data interrupt.
ISR(INT2_vect){
  new_packet_flag = 1;
}

