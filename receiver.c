#include <avr/io.h>
#include <stdio.h>
#include "m_general.h"
#include "timer.h"
#include "comm.h"
#include "camera.h"
#include "debug.h"

#define RXADDRESS 100

volatile int new_camera_data_flag = 0;
volatile int new_packet_flag = 0;
unsigned int blobs[12];

int main(void) {
  // set system clock to 16MHz.
  m_clockdivide(0);

  // enable global interrupts.
  sei();

  // usb data channel init.
  debug_init();

  // rf communicaiton init.
  comm_init(RXADDRESS);

  while(1) {
    if (new_packet_flag) {
      m_green(TOGGLE);
      comm_handler();
      new_packet_flag = 0;
    }
  }
  return 0;
}

// rf data interrupt.
ISR(INT2_vect){
  new_packet_flag = 1;
}
