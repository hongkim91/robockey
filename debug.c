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

void send_int(char *label, int x) {
  sprintf(buf, "%s: %d\n", label, x);
  send_buf(buf);
}

void send_blobs(unsigned int *blobs) {
  sprintf(buf, "[%d,%d]/[%d,%d]/[%d,%d]/[%d,%d]\n",
          blobs[0], blobs[1], blobs[3], blobs[4],
          blobs[6], blobs[7], blobs[9], blobs[10]);
  send_buf(buf);
}

void wait_matlab() {
  while(!m_usb_rx_available());
  m_usb_rx_flush();
}

void send_matlab_blobs(unsigned int *blobs) {
  sprintf(buf, "%d \t %d \t %d \t %d \t %d \t %d \t %d \t %d\n",
          blobs[0], blobs[1], blobs[3], blobs[4],
          blobs[6], blobs[7], blobs[9], blobs[10]);
  send_buf(buf);
}

void send_matlab_robot(POINT* robot) {
  sprintf(buf, "%d \t %d\n", robot->x, robot->y);
  send_buf(buf);
}

void send_matlab_localization(unsigned int *blobs) {
  POINT *robot = determine_position(blobs);
  rotate_blobs(blobs, robot->theta);
  wait_matlab();
  send_matlab_blobs(blobs);
  send_matlab_robot(robot);
  free(robot);
}
