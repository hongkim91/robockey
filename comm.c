#include <avr/io.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "debug.h"

#define CHANNEL 1
#define PACKET_LENGTH 10
#define TXADDRESS 100

char buffer[PACKET_LENGTH] = {0,0,0,0,0,0,0,0,0,0};

void comm_init(int rx_addr) {
  m_bus_init();
  if (!m_rf_open(CHANNEL, rx_addr, PACKET_LENGTH)) {
    m_red(ON);
  }
}

void comm_handler() {
  m_rf_read(buffer,PACKET_LENGTH);
  send_hex("buf[0]", buffer[0]);
}

void send_camera_data(unsigned int* blobs) {
  int i, j=0;
  for (i=0; i<12; i=i+3) {
    buffer[j] = blobs[i];
    buffer[j+1] = blobs[i+1];
    j = j+2;
  }
  m_rf_send(TXADDRESS, buffer, PACKET_LENGTH);
}

