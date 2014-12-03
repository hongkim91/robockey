#include "m_general.h"
#include "motor.h"
#include "adc.h"
#include "timer.h"
#include <math.h>
#include "m_usb.h"
#include "debug.h"

/* float KP = .3; //proportional term */
/* float KD = 1.5; // derivative term */
float KP = .25; //proportional term
float KD = 0.0; // derivative term
bool have_puck = FALSE;

int error_l; //error on left motor
int error_r; // error on right motor
int prev_error_l = 0; //left motor previous error
int prev_error_r = 0; // right motor previous error

void motor_init() {
  // pwm enable pins.
  set(DDRB,5);
  set(DDRB,6);

  // direction pins.
  set(DDRB,4);
  set(DDRC,6);
  set(PORTB, 4);
  set(PORTC, 6);
}

void rotate(int direction) {
  switch (direction) {
  case CLOCKWISE:
    set(PORTB, 4);
    clear(PORTC, 6);
    break;
  case CCLOCKWISE:
    clear(PORTB, 4);
    set(PORTC, 6);
    break;
  }
}

void stop() {
    OCR1A = 0;
    OCR1B = 0;
}

void go_forward() {
  clear(PORTB, 4);
  clear(PORTC, 6);
}

bool find_puck() {
  int tgt_duty_cycle_L = 200;
  int tgt_duty_cycle_R = 200;

  int sensor_b_l = filtered_sensor_values[3];
  int sensor_t_l = filtered_sensor_values[4];
  int sensor_middle = filtered_sensor_values[0];
  int sensor_t_r = filtered_sensor_values[1];
  int sensor_b_r = filtered_sensor_values[2];

  if (sensor_middle>=1010) {
    have_puck = TRUE;
    m_red(ON);
  }

  if (!have_puck) {
    if (sensor_b_l >= sensor_t_l || sensor_b_r>=sensor_t_r) {
      set_motor_duty_cycle(200, 200);
      rotate(CLOCKWISE);
    } else {
      error_l = sensor_t_l - sensor_t_r;
      error_r = sensor_t_r - sensor_t_l;

      tgt_duty_cycle_R += KP*error_l + KD*(error_l - prev_error_l);
      tgt_duty_cycle_L += KP*error_r + KD*(error_r - prev_error_r);

      set_motor_duty_cycle(tgt_duty_cycle_R, tgt_duty_cycle_L);
      prev_error_r = error_r;
      prev_error_l = error_l;
      go_forward();
    }
  }
  return have_puck;
}

void set_motor_duty_cycle(int refA, int refB) {
  if (refA > 255) {
    OCR1A = 255;
  } else {
    OCR1A = refA;
  }
  if (refB > 255) {
    OCR1B = 255;
  } else {
    OCR1B = refB;
  }
}
