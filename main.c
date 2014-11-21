#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include "m_general.h"
#include "m_wii.h"
#include "m_usb.h"
#include "timer.h"
#include "localization.h"

volatile int new_camera_data_flag = 0;
unsigned int blobs[12];
char buf[100];

void send_str(char *str);
void send_float(char *label, float value);

int main(void) {
  m_clockdivide(0);

  sei();

  m_usb_init();
  while(!m_usb_isconnected()); // wait for a connection
  m_usb_tx_string("USB connected.\n");

  init_timer1();

  m_wii_open();

  while(1) {

    if (new_camera_data_flag) {
      m_red(TOGGLE);

      if (m_wii_read(blobs)) {
        sprintf(buf, "[%d,%d]/[%d,%d]/[%d,%d]/[%d,%d]\n",
                blobs[0], blobs[1], blobs[3], blobs[4],
                blobs[6], blobs[7], blobs[9], blobs[10]);
        send_str(buf);
      } else {
        send_str("failed reading from mWii");
      }

      FPOINT *p = determine_position(blobs);
      send_float("x", p->x);
      send_float("y", p->y);
      free(p);

      new_camera_data_flag = 0;
    }
  }
  return 0;
}

ISR(TIMER1_COMPA_vect)
{
  new_camera_data_flag = 1;
}

void send_str(char *str){
  int i;
  for (i=0; i< strlen(buf); i++) {
    m_usb_tx_char(*(buf+i));
  }
}

void send_float(char *label, float value){
  sprintf(buf, "%s: %.3f\n", label, value);
  send_str(buf);
}
