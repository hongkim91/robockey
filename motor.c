#include "m_general.h"
#include "motor.h"
#include "adc.h"
#include "timer.h"
#include <math.h>
#include "m_usb.h"
#include "debug.h"

float KP = .3; //proportional term
float KD = 1.5; // derivative term
// float KI = 0.00005; // integral term
float KI = 0.0; // integral term
bool have_puck = FALSE;
int set_point = 0;
int prev_error_l = 0; //left motor previous error
int prev_error_r = 0; // right motor previous error
int ierror_l = 0;// integral error left
int ierror_r = 0;// integral error right
int oc_max = 255;
int speed;



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

int find_puck() {
  sensor_b_l = filtered_sensor_values[3];
  sensor_t_l = filtered_sensor_values[4];
  sensor_middle = filtered_sensor_values[0];
  sensor_t_r = filtered_sensor_values[1];
  sensor_b_r = filtered_sensor_values[2];

 

  // if (sensor_middle >=1000) {
  //   oc_max = 215;
  // }

  if (sensor_middle>=1010) {
    have_puck = TRUE;
    m_red(ON);
    // OCR1A = 0;
    // OCR1B = 0;

  }

  if (!have_puck) {
    if (sensor_b_l >= sensor_t_l || sensor_b_r >=sensor_t_r) {
      OCR1A = 210;
      OCR1B = 210;
      rotate(1);
    } else {

      error_l = set_point - (sensor_t_l- sensor_t_r);
      error_r = set_point - (sensor_t_r-sensor_t_l);

      derror_r = error_r - prev_error_r;
      derror_l = error_l - prev_error_l;

      c_l = KP*error_l + KD*derror_l + sensor_middle/5 ;
      c_r = KP*error_r + KD*derror_r  + sensor_middle/5;

      if (c_l > oc_max) {
        OCR1A = oc_max;
      } else {
        OCR1A =  c_l ;
      }
      if ( c_r > oc_max) {
        OCR1B = oc_max;
      } else{
        OCR1B = c_r;
      }
      prev_error_r = error_r;
      prev_error_l = error_l;
      m_usb_tx_int(OCR1A);
      m_usb_tx_string("\t");
      m_usb_tx_int(OCR1B);

      m_usb_tx_string("\n");
      // ierror_r = ierror_r + error_r;
      // ierror_l = ierror_l + error_l;
      go_forward();
    }
  }
  return have_puck;
}

int map_value(int input,int in_min, int in_max, int out_min, int out_max)
{
  int input_range = in_max - in_min;
  int output_range = out_max - out_min;
  int output;

  output = (input - in_min)*output_range / input_range + out_min;
  return output;
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
