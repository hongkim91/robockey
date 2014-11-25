#include <stdio.h>
#include "m_general.h"
#include "m_wii.h"
#include "debug.h"
#include "localization.h"

char buf[100];

void camera_init() {
  if (!m_wii_open()) {
    m_red(ON);
  }
}

void camera_handler(unsigned int *blobs, float *x, float *y) {
  if (m_wii_read(blobs)) {
    sprintf(buf, "[%d,%d]/[%d,%d]/[%d,%d]/[%d,%d]\n",
            blobs[0], blobs[1], blobs[3], blobs[4],
            blobs[6], blobs[7], blobs[9], blobs[10]);
    send_buf(buf);
  } else {
    m_usb_tx_string("failed reading from mWii");
  }

  FPOINT *p = determine_position(blobs);
  send_float("x", p->x);
  send_float("y", p->y);
  *x = p->x;
  *y = p->y;
  free(p);
}
