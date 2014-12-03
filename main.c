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

#define RXADDRESS 84

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
  /* comm_init(RXADDRESS); */

  while(1) {
     update_ADC(); 
     filter_sensor_values(); 
     find_puck(); 
     // print_filtered_values();

   

    // m_usb_tx_int(OCR1A);
    // m_usb_tx_string(" \n ");
    // m_usb_tx_int(OCR1B);

  //   if (new_camera_data_flag) {
  //     camera_handler(blobs);
  //     new_camera_data_flag = 0;
  //   }

  //   if (new_packet_flag) {
  //     m_red(TOGGLE);
  //     comm_handler();
  //     new_packet_flag = 0;
  //   }
  // }
  // return 0;
       }
     }



// camera interrupt.
ISR(TIMER3_COMPA_vect) {
  new_camera_data_flag = 1;
}

// rf data interrupt.
/* ISR(INT2_vect){ */
/*   new_packet_flag = 1; */
/* } */

//adc interrupt
ISR(ADC_vect)
{  // this interrupt is present to clear ADCSRA: ADIF flag (in this
    // case write a logivcal one to it)
}


