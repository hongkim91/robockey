#include <avr/io.h>
#include <math.h>
#include "m_general.h"
#include "localization.h"

#define SYSTEM_CLOCK 16*pow(10,6)
#define POLLING_FREQ 10

// this timer will pull values from the imu when it overflows
void init_timer1()
{
  float timer_freq;

  /* // set prescaler to /8 */
  /* clear(TCCR1B,CS12); */
  /* set(TCCR1B,CS11); */
  /* clear(TCCR1B,CS10); */
  /* // timer freq = 16MHz /8 = 2MHz */
  /* timer_freq = SYSTEM_CLOCK/8; */

  // set prescaler to /1024
  set(TCCR1B,CS12);
  clear(TCCR1B,CS11);
  set(TCCR1B,CS10);
  // timer freq = 16MHz /1024 = 16kHz
  timer_freq = SYSTEM_CLOCK/1024;

  // set timer mode: (mode 4) UP to OCR1A
  clear(TCCR1B,WGM13);
  set(TCCR1B,WGM12);
  clear(TCCR1A,WGM11);
  clear(TCCR1A, WGM10);

  // set OCR1A so that interrupt would occur at POLLING_FREQ.
  OCR1A =  timer_freq/POLLING_FREQ;

  // enable interrput when timer is OCR1A
  set(TIMSK1,OCIE1A);
}

/* void camera_data_handler(unsigned int *blobs) { */
/*   FPOINT *p = determine_position(blobs); */
/*   m_green(TOGGLE); */
/* } */
