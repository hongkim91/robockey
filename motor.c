#include "m_general.h"

void motor_init() {
  // pwm enable pins.
  set(DDRB,5);
  set(DDRB,6);
  /* set(PORTB, 5); */
  /* set(PORTB, 6); */

  // direction pins.
  set(DDRB,4);
  set(DDRC,6);
  set(PORTB, 4);
  set(PORTC, 6);
}
