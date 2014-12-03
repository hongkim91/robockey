#include "m_general.h"
#include "motor.h"
#include "adc.h"
#include "timer.h"
#include <math.h>
#include "m_usb.h"

float KP = .25; //proportional term
float KD = 0.0; // derivative term
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

void find_puck() {
  sensor_b_l = filtered_sensor_values[3];
  sensor_t_l = filtered_sensor_values[4];
  sensor_middle = filtered_sensor_values[0];
  sensor_t_r = filtered_sensor_values[1];
  sensor_b_r = filtered_sensor_values[2];

  int a_error_l;
  int a_error_r;
  int a_derror_l;
  int a_derror_r;
  int a_speed;



  speed = (sensor_t_l + sensor_t_r)/ 2;

  if (!have_puck)
  {
      error_l = set_point - (sensor_t_l- sensor_t_r);
      error_r = set_point - (sensor_t_r-sensor_t_l);
      derror_r = error_r - prev_error_r;
      derror_l = error_l - prev_error_l;
      speed = (sensor_t_l + sensor_t_r)/ 2;
      prev_error_l = error_l;
      prev_error_r = error_r;

      m_usb_tx_int(error_l);
      m_usb_tx_string("error left \n");
      // m_usb_tx_int(error_r);
      // m_usb_tx_string("\n");
      // m_usb_tx_int(derror_l);
      // m_usb_tx_string("\n");
      // m_usb_tx_int(derror_r);
      // m_usb_tx_string("\n");

      a_error_l = map_value(error_l,-750,750,0,255);
      a_error_r = map_value(error_r,-850,850,0,255);
      a_derror_l = map_value(derror_l,-30,30,0,255);
      a_derror_r = map_value(derror_r,-30,30,0,255);
      a_speed = map_value(speed,0,2046,0,255);
      m_usb_tx_int(a_error_l);
      m_usb_tx_string("a_error left \n");

      OCR1A =  a_speed + KP*a_error_r + KD*a_derror_r;
      OCR1B =  a_speed + KP*a_error_l + KD*a_derror_l;
  }

  if(sensor_middle >= 1000){
    have_puck = TRUE;
    OCR1B = 0;
    OCR1A = 0;
  }



// Old code-- Testing new
  // if (sensor_middle >=990) {
  //   oc_max = 215;
  //   KI = 0.001;
  // }

  // if (sensor_middle>=1010) {
  //   have_puck = TRUE;
  //   m_red(ON);
  //   OCR1A = 0;
  //   OCR1B = 0;

  // }

  // if (!have_puck) {
  //   if (sensor_b_l >= sensor_t_l || sensor_b_r>=sensor_t_r) {
  //     OCR1A = 200;
  //     OCR1B = 200;
  //     rotate(1);
  //     go();
  //   } else {

  //     error_l = set_point - (sensor_t_l- sensor_t_r);
  //     error_r = set_point - (sensor_t_r-sensor_t_l);

  //     derror_r = error_r - prev_error_r;
  //     derror_l = error_l - prev_error_l;

  //     c_l = KP*error_l + KD*derror_l + KI*ierror_l;
  //     c_r = KP*error_r + KD*derror_r + KI*ierror_r;

  //     if (OCR1A + c_r > oc_max) {
  //       OCR1A = oc_max;
  //     } else {
  //       OCR1A = OCR1A + c_r ;
  //     }
  //     if (OCR1B + c_l > oc_max) {
  //       OCR1B = oc_max;
  //     } else{
  //       OCR1B = OCR1B + c_l;
  //     }
  //     prev_error_r = error_r;
  //     prev_error_l = error_l;
  //     ierror_r = ierror_r + error_r;
  //     ierror_l = ierror_l + error_l;
  //     go_forward();
  //   }
  // }
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
