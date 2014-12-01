#include "m_general.h"
#include "motor.h"
#include "adc.h"
#include "timer.h"
#include <math.h>

float KP = .22; //proportional term
float KD = 0.0005; // derivative term
float KI = 0.0033; // integral term
bool have_puck = FALSE;
int set_point = 0;
int prev_error_l = 0; //left motor previous error
int prev_error_r = 0; // right motor previous error
int ierror_l = 0;// integral error left
int ierror_r = 0;// integral error right




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
  if (check(PINB, 5)) {
    clear(PORTB, 5);
    clear(PORTB, 6);
  }
}

void go() {
  if (!check(PINB, 5)) {
    set(PORTB, 5);
    set(PORTB, 6);
  }
}

void go_forward() {
  clear(PORTB, 4);
  clear(PORTC, 6);
  go();
}


void find_puck() {

    sensor_b_l = filtered_sensor_values[0];
    sensor_t_l = filtered_sensor_values[1];
    sensor_middle = filtered_sensor_values[2];
    sensor_t_r = filtered_sensor_values[3];
    sensor_b_r = filtered_sensor_values[4];

if(sensor_middle > 999)
{
  have_puck = TRUE;
  OCR1A = 130;
  OCR1B = 135;

}

if(!have_puck)
{
  if(sensor_b_l >= sensor_t_l || sensor_b_r>=sensor_t_r)
  {
    OCR1A = 220;
    OCR1B = 220;
    rotate(1);
    go();
  }

  else{

    error_l = set_point - (sensor_t_l- sensor_t_r);
    error_r = set_point - (sensor_t_r-sensor_t_l);

    derror_r = error_r - prev_error_r;
    derror_l = error_l - prev_error_l;

    c_l = KP*error_l + KD*derror_l + KI*ierror_l;
    c_r = KP*error_r + KD*derror_r + KI*ierror_r;

    if(OCR1A + c_r > 255)
    {
      OCR1A = 255;
    }

    else{
      OCR1A = OCR1A + c_r - 0.45*sensor_middle;
    }
    if(OCR1B + c_l > 255)
    {
      OCR1B = 255;
    }
    else{
      OCR1B = OCR1B + c_l - 0.45*sensor_middle;
    }
    prev_error_r = error_r;
    prev_error_l = error_l;
    ierror_r = ierror_r + error_r;
    ierror_l = ierror_l + error_l;
    go_forward();
  }

}

}





