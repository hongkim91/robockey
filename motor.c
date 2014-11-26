#include "m_general.h"
#include "motor.h"

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
