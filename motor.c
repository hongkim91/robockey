#include "m_general.h"
#include "features.h"
#include "motor.h"
#include "adc.h"
#include "timer.h"
#include <math.h>
#include "m_usb.h"
#include "debug.h"
#include "control.h"

#define PUCK_SPEED_LIMIT 190.0

bool first_run = FALSE;
int prev_theta_est = 0;
int theta_est_cutoff = 0;

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
  set(PORTB, 4);
  set(PORTC, 6);
}

//refA: left
//refB: right

void set_motor_duty_cycle(int refA, int refB) {
  /* int temp; */
  /* temp = refA; */
  /* refA = refB; */
  /* refB = temp; */

  if (refA > 255) {
    OCR1A = 255;
    set(PORTB, 4);
  } else if (refA < -255) {
    OCR1A = 255;
    clear(PORTB, 4);
  } else if (refA < 0) {
    OCR1A = abs(refA);
    clear(PORTB, 4);
  } else {
    OCR1A = refA;
    set(PORTB, 4);
  }
  if (refB > 255) {
    OCR1B = 255;
    set(PORTC, 6);
  } else if (refB < -255) {
    OCR1B = 255;
    clear(PORTC, 6);
  } else if (refB < 0) {
    OCR1B = abs(refB);
    clear(PORTC, 6);
  } else {
    OCR1B = refB;
    set(PORTC, 6);
  }

  /* send_float("OCR1A", OCR1A); */
  /* send_float("direction left", check(PINB, 4)); */
  /* send_float("OCR1B", OCR1B); */
  /* send_float("direction right", check(PINC, 6)); */
}

/* bool have_puck() { */
/*    // SENSOR ON INDEX 0 IS ASSUMED MIDDLE. */
/*   int sensor_middle = sensor_values[0]; */
/*   if (sensor_middle>=1010) { */
/*     return TRUE; */
/*   } else { */
/*     return FALSE; */
/*   } */
/* } */

bool have_puck() {
  if (FIND_PUCK) {
    return !check(PINB, 0) || !check(PINB, 1);
  } else {
    return TRUE;
  }
}

void find_puck() {
  float KP = .1;
  float KD = 1;

  int theta_est = estimate_puck_theta();
  if (!first_run) {
    prev_theta_est = theta_est;
    theta_est_cutoff = 1000;
    first_run = TRUE;
  }

  int turn = KP * theta_est + KD *(theta_est - prev_theta_est);
  prev_theta_est = theta_est;
  int speed_val = speed(theta_est);
  if (speed_val > PUCK_SPEED_LIMIT - abs(turn)) {
    speed_val = PUCK_SPEED_LIMIT - abs(turn);
  }

  int tgt_duty_cycle_L = speed_val + turn;
  int tgt_duty_cycle_R = speed_val - turn;

  m_usb_tx_string("--------------------FIND PUCK --------------------\n");
  set_motor_duty_cycle(tgt_duty_cycle_L, tgt_duty_cycle_R);

  send_float("theta_est", theta_est);
  send_float("turn", turn);
  send_float("speed", speed_val);

  send_float("tgt_duty_cycle_R", tgt_duty_cycle_R);
  send_float("tgt_duty_cycle_L", tgt_duty_cycle_L);
}

int speed(int theta_est) {
  int sensor_b_l = sensor_values[2];
  int sensor_t_l = sensor_values[1];
  int sensor_t_r = sensor_values[4];
  int sensor_b_r = sensor_values[3];

  switch (determine_quadrant()) {
  case T_R:
    if (sensor_t_l > sensor_b_r) {
      return (PUCK_SPEED_LIMIT -
              (PUCK_SPEED_LIMIT/(float) abs(theta_est_cutoff)) * theta_est);
    } else {
      theta_est_cutoff = theta_est;
      return 0;
    }
  case T_L:
    if (sensor_t_r > sensor_b_l) {
      return (PUCK_SPEED_LIMIT +
              (PUCK_SPEED_LIMIT/(float) abs(theta_est_cutoff)) * theta_est);
    } else {
      theta_est_cutoff = theta_est;
      return 0;
    }
  case B_R:
  case B_L:
    return 0;
  }
  return 0; // shouldn't happen.
}
