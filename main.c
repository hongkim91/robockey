#include <avr/io.h>
#include <stdio.h>
#include "m_general.h"
#include "timer.h"
#include "comm.h"
#include "camera.h"
#include "debug.h"
#include "motor.h"
#include "localization.h"
#include "adc.h"

#define RXADDRESS 84

#define NOTSET 0
#define RIGHT 1
#define LEFT 2

#define TOLERANCE 0.2

volatile int new_camera_data_flag = 0;
volatile int new_packet_flag = 0;
unsigned int blobs[12];

float x;
float y;
float robot_theta;
float goal_theta;
POINT *robot_goal[2];

int goal = NOTSET;
int count = 0;
int found = 0;

int determine_goal();

int main(void) {
  // set system clock to 16MHz.
  m_clockdivide(0);

  // enable global interrupts.
  sei();

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

  //motors timer
  init_timer1();

  // rf communicaiton init.
  /* comm_init(RXADDRESS); */

  // robot_goal[1] = create_point(820, 350);

  while(1) {
    update_ADC();
    filter_sensor_values();
    find_puck();
    // print_filtered_values();

    // m_usb_tx_int(OCR1A);
    // m_usb_tx_string(" \n ");
    // m_usb_tx_int(OCR1B);

    if (new_camera_data_flag) {
      camera_handler(blobs, &x, &y, &robot_theta);

      /* if (!determine_goal()) { */
      /*   continue; */
      /* } */

      if (x > 850) {
        stop();
        continue;
      }

      robot_goal[0] = create_point(x,y);
      goal_theta = determine_angle(robot_goal);
      //      if (found || fabs(robot_theta - goal_theta) < 0.40) {
      if (found || fabs(robot_theta - goal_theta) < TOLERANCE) {
        m_red(ON);
        m_green(OFF);
        /* stop(); */
        set_motor_voltage(255);
        go_forward();
        /* found = 1; */
      } else if ((robot_theta - goal_theta) > TOLERANCE)  {
        m_red(OFF);
        m_green(ON);
        set_motor_voltage(50);
        rotate(CLOCKWISE);
        go();
      } else {
        m_red(OFF);
        m_green(ON);
        set_motor_voltage(50);
        rotate(CCLOCKWISE);
        go();
      }

      send_camera_data(blobs, robot_theta*57.3, goal_theta*57.3);

      send_float("robot_theta", robot_theta * 57.3);
      send_float("goal_theta", goal_theta * 57.3);
      free(robot_goal[0]);

      /* send_camera_data(blobs, x, y); */
      new_camera_data_flag = 0;
    }
    if (new_packet_flag) {
      m_red(TOGGLE);
      comm_handler();
      new_packet_flag = 0;
    }
  }
  return 0;
}

int determine_goal() {
  if (!goal && count > 60) {
    if (x > 500) {
      /* send_point(x,y); */
      /* m_red(ON); */
      goal = RIGHT;
      robot_goal[1] = create_point(820, 350);
    } else {
      /* send_point(x,y); */
      /* m_green(ON); */
      goal = LEFT;
      robot_goal[1] = create_point(200, 350);
    }
  } else {
    count++;
  }

  if (goal) {
    return 1;
  } else {
    return 0;
  }
}

// camera interrupt.
ISR(TIMER3_COMPA_vect) {
  new_camera_data_flag = 1;
}

// rf data interrupt.
/* ISR(INT2_vect){ */
/*   new_packet_flag = 1; */
/* } */

//adc interrupt
ISR(ADC_vect)
{  // this interrupt is present to clear ADCSRA: ADIF flag (in this
    // case write a logivcal one to it)
}


