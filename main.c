#include <avr/io.h>
#include <stdio.h>
#include "m_general.h"
#include "timer.h"
#include "comm.h"
#include "camera.h"
#include "debug.h"

volatile int new_camera_data_flag = 0;
volatile int new_packet_flag = 0;
unsigned int blobs[12];

void send_str(char *str);
void send_float(char *label, float value);

int main(void) {
  // set system clock to 16MHz.
  m_clockdivide(0);

  // enable global interrupts.
  sei();

  // usb data channel init.
  debug_init();

  // camera polling timer.
  init_timer1();

  // mWii init.
  camera_init();

  // rf communicaiton init.
  comm_init();

  while(1) {
    if (new_camera_data_flag) {
      m_red(TOGGLE);
      camera_handler(blobs);
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
ISR(TIMER1_COMPA_vect) {
  new_camera_data_flag = 1;
}

// rf data interrupt.
ISR(INT2_vect){
  new_packet_flag = 1;
}
