#include <string.h>
#include <stdio.h>
#include "m_usb.h"
#include "localization.h"

char buf[100];

void debug_init() {
  m_usb_init();
  /* while(!m_usb_isconnected()); // wait for a connection */
  m_usb_tx_string("USB is connected.\n");
}

void send_buf(char *buf) {
  int i;
  for (i=0; i< strlen(buf); i++) {
    m_usb_tx_char(*(buf+i));
  }
}

void send_float(char *label, float value) {
  sprintf(buf, "%s: %.3f\n", label, value);
  send_buf(buf);
}

void send_hex(char *label, char value) {
  sprintf(buf, "%s: %X\n", label, (unsigned)(unsigned char) value);
  send_buf(buf);
}

void send_point(int x, int y) {
  sprintf(buf, "x: %d, y: %d\n", x, y);
  send_buf(buf);
}
