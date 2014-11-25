#include <avr/io.h>
#include <stdio.h>
#include "m_general.h"
#include "timer.h"
#include "comm.h"
#include "camera.h"
#include "debug.h"

#define RXADDRESS 84

volatile int new_camera_data_flag = 0;
volatile int new_packet_flag = 0;
unsigned int blobs[12];

float x;
float y;

int main(void) {
  // set system clock to 16MHz.
  m_clockdivide(0);

  // enable global interrupts.
  sei();

  // usb data channel init.
  debug_init();

  //MOTOR DIRECTION PIN
  set(DDRB,4);
  set(DDRC,6);
  set(PORTB, 4);
  set(PORTC, 6);

  // motor enable pin pwm timer.
  // FIXME: Bringing down the prescalar of timer1 causes problems with mWii.
  /* init_timer1(); */

  // camera polling timer.
  init_timer3();

  // mWii init.
  camera_init();

  // rf communicaiton init.
  comm_init(RXADDRESS);

  while(1) {
    if (new_camera_data_flag) {
      m_red(TOGGLE);
      camera_handler(blobs, &x, &y);
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

