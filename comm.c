#include <avr/io.h>
#include <stdio.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "debug.h"
#include "string.h"

#define CHANNEL 1
#define PACKET_LENGTH 10
#define TXADDRESS 100

char buffer[PACKET_LENGTH] = {0,0,0,0,0,0,0,0,0,0};
char buf[100];

void comm_init(int rx_addr) {
  m_bus_init();
  if (!m_rf_open(CHANNEL, rx_addr, PACKET_LENGTH)) {
    m_red(ON);
  }
}

void comm_handler() {
  m_rf_read(buffer,PACKET_LENGTH);
  /* send_hex("buf[0]", buffer[0]); */
}

void receiver_handler() {
  m_rf_read(buffer,PACKET_LENGTH);
  float *b = (float *) buffer;
  sprintf(buf, "rt:%.3f \t gt:%.3f\n", b[0], b[1]);
  send_buf(buf);
}

void send_camera_data(unsigned int* blobs, float x, float y) {
  memcpy(buffer, &x, 4);
  memcpy(buffer+4, &y, 4);
  m_rf_send(TXADDRESS, buffer, PACKET_LENGTH);
}

void live_plot_data() {
  float *b = (float *) buffer;
  sprintf(buf, "%.3f \t %.3f\n", b[0], b[1]);
  send_buf(buf);
}
