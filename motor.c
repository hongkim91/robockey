#include <stdlib.h>
#include "m_general.h"
#include "features.h"
#include "motor.h"
#include "timer.h"

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
  set_motor_duty_cycle(0,0);
}

//refA: left
//refB: right
void set_motor_duty_cycle(int refA, int refB) {
  if (TEST_GO_FORWARD) {
    refA = 200;
    refB = 200;
  } else if (TEST_GO_BACKWARD) {
    refA = -200;
    refB = -200;
  }

  if (refA > 255) {
    OCR1A = 255;
    set(PORTB, 4);
  } else if (refA < -255) {
    OCR1A = 255;
    clear(PORTB, 4);
  } else if (refA < 0) {
    OCR1A = abs(refA);
    clear(PORTB, 4);
  } else {
    OCR1A = refA;
    set(PORTB, 4);
  }

  if (refB > 255) {
    OCR1B = 255;
    set(PORTC, 6);
  } else if (refB < -255) {
    OCR1B = 255;
    clear(PORTC, 6);
  } else if (refB < 0) {
    OCR1B = abs(refB);
    clear(PORTC, 6);
  } else {
    OCR1B = refB;
    set(PORTC, 6);
  }
}
