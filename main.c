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
#include "control.h"

#define RXADDRESS 84
/* #define RXADDRESS 85 */

volatile int camera_timer_flag = 0;
volatile int new_packet_flag = 0;
POINT *robot = NULL;

void m2_init();

int main(void) {
  m2_init();

  while(1) {
    update_ADC();
    set_motor_duty_cycle(200,200);

    /* if (!is_play()) { */
    /*   set_motor_duty_cycle(0,0); */
    /* } */

    /* if (is_play() && !have_puck()) { */
    /*   find_puck(); */
    /* } */

    /* if (camera_timer_flag) { */
    /*   if (!set_goal() || have_puck()) { */
    /*     robot = localize_robot(); */
    /*   } */
    /*   if (is_play() && set_goal() && have_puck()) { */
    /*     drive_to_goal(robot); */
    /*   } */
    /*   camera_timer_flag = 0; */
    /* } */

    /* if (new_packet_flag) { */
    /*   comm_handler(); */
    /*   new_packet_flag = 0; */
    /* } */
  }
  return 0;
}

void m2_init() {
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

  //motors timer
  init_timer1();

  // rf communicaiton init.
  comm_init(RXADDRESS);
}

// camera interrupt.
ISR(TIMER3_COMPA_vect) {
  camera_timer_flag = 1;
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
