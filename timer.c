#include <avr/io.h>
#include <math.h>
#include "m_general.h"
#include "localization.h"
#include "timer.h"

// motor enable pin pwm timer.
void init_timer1()
{
  // set prescaler to /256
  // timer freq = 16MHz
  set(TCCR1B,CS12);
  clear(TCCR1B,CS11);
  clear(TCCR1B,CS10);

  // set timer mode: (mode 5) UP to 0x00FF (8-bit), PWM mode
  // pwm freq = timer freq / 256 = 244Hz
  clear(TCCR1B,WGM13);
  set(TCCR1B,WGM12);
  clear(TCCR1A,WGM11);
  set(TCCR1A,WGM10);

  // set OCR1A and OCR1B ceilings
  OCR1A = 200;
  OCR1B = 200;

  // OCR1A output compare: B5 clear at OCR1A, set at rollover
  set(TCCR1A, COM1A1);
  clear(TCCR1A, COM1A0);

  // OCR1B output compare: B6 clear at OCR1B, set at rollover
  set(TCCR1A, COM1B1);
  clear(TCCR1A, COM1B0);
}

// camera polling timer.
void init_timer3() {
  float timer_freq;

  // set prescaler to /1024
  set(TCCR3B,CS32);
  clear(TCCR3B,CS31);
  set(TCCR3B,CS30);
  // timer freq = 16MHz /1024 = 16kHz
  timer_freq = SYSTEM_CLOCK/1024;

  // set timer mchode: (mode 4) UP to OCR1A
  clear(TCCR3B,WGM33);
  set(TCCR3B,WGM32);
  clear(TCCR3A,WGM31);
  clear(TCCR3A, WGM30);

  // set OCR1A so that interrupt would occur at POLLING_FREQ.
  OCR3A =  timer_freq/POLLING_FREQ;

  // enable interrput when timer is OCR1A
  set(TIMSK3,OCIE3A);
}

