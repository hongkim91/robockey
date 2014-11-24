#include <avr/io.h>
#include <stdio.h>
#include "m_general.h"
#include "timer.h"
#include "comm.h"
#include "camera.h"
#include "debug.h"

#define RXADDRESS 100

int main(void) {
  char rx_buffer; //computer interactions

  // set system clock to 16MHz.
  m_clockdivide(0);

  // enable global interrupts.
  sei();

  // usb data channel init.
  debug_init();

  // rf communicaiton init.
  comm_init(RXADDRESS);

  while(1) {
    while(!m_usb_rx_available());  	//wait for an indication from the computer
    rx_buffer = m_usb_rx_char();  	//grab the computer packet
    m_usb_rx_flush();  		//clear buffer

    if(rx_buffer == 1) {
      live_plot_data();
    }
  }
  return 0;
}

// rf data interrupt.
ISR(INT2_vect){
  m_green(TOGGLE);
  receiver_handler();
}
