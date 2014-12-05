#include "m_general.h"
#include "motor.h"
#include "adc.h"
#include "timer.h"
#include <math.h>
#include "m_usb.h"
#include "debug.h"

/* float KP = .3; //proportional term */
/* float KD = 1.5; // derivative term */
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

// B4: right
// C6: left
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
  /* float KP = .10; //proportional term */
  /* float KD = 1; // derivative term */
  float KP = .15; //proportional term
  float KD = .7; // derivative term

  int tgt_duty_cycle_L = 170;
  int tgt_duty_cycle_R = 170;

  int sensor_b_l = filtered_sensor_values[3];
  int sensor_t_l = filtered_sensor_values[4];
  int sensor_middle = filtered_sensor_values[0];
  int sensor_t_r = filtered_sensor_values[1];
  int sensor_b_r = filtered_sensor_values[2];

  have_puck = FALSE;

  if (sensor_middle>=1010) {
    have_puck = TRUE;
    /* set_motor_duty_cycle(0,0); */
    m_red(ON);
  }

  if (!have_puck) {
    if (sensor_middle > 150) {// ||
        /* ((sensor_t_l + sensor_t_r) > (sensor_b_l + sensor_b_r))) { */
      tgt_duty_cycle_L = 130;
      tgt_duty_cycle_R = 130;
    } else {
      KP = (200 - sensor_middle)/100 * KP;
      KD = 5;
      /* tgt_duty_cycle_L = sensor_middle-50; */
      /* tgt_duty_cycle_R = sensor_middle-50; */
      tgt_duty_cycle_L = 0;
      tgt_duty_cycle_R = 0;
    }

    if (sensor_t_l > 700 || sensor_t_r > 700) {
      error_l = sensor_t_l - sensor_t_r;
      error_r = sensor_t_r - sensor_t_l;
    } else {
      error_l = (sensor_t_l + sensor_b_l) - (sensor_t_r + sensor_b_r);
      error_r = (sensor_t_r + sensor_b_r) - (sensor_t_l + sensor_b_l);
    }

    tgt_duty_cycle_R += KP*error_l + KD*(error_l - prev_error_l);
    tgt_duty_cycle_L += KP*error_r + KD*(error_r - prev_error_r);

    send_float("sensor_middle", sensor_middle);
    send_float("error_r", error_r);
    send_float("error_l", error_l);
    send_float("tgt_duty_cycle_R", tgt_duty_cycle_R);
    send_float("tgt_duty_cycle_L", tgt_duty_cycle_L);

    set_motor_duty_cycle(tgt_duty_cycle_R, tgt_duty_cycle_L);
    prev_error_r = error_r;
    prev_error_l = error_l;
  }
  return have_puck;
}

//refA: right
//refB: left

void set_motor_duty_cycle(int refA, int refB) {
  if (refA > 255) {
    OCR1A = 255;
    clear(PORTB, 4);
  } else if (refA < -255) {
    OCR1A = 255;
    set(PORTB, 4);
  } else if (refA < 0) {
    OCR1A = abs(refA);
    set(PORTB, 4);
  } else {
    OCR1A = refA;
    clear(PORTB, 4);
  }
  if (refB > 255) {
    OCR1B = 255;
    clear(PORTC, 6);
  } else if (refB < -255) {
    OCR1B = 255;
    set(PORTC, 6);
  } else if (refB < 0) {
    OCR1B = abs(refB);
    set(PORTC, 6);
  } else {
    OCR1B = refB;
    clear(PORTC, 6);
  }
  send_float("OCR1A", OCR1A);
  send_float("direction", check(PINB, 4));
  send_float("OCR1B", OCR1B);
  send_float("direction", check(PINC, 6));
}
