#include <avr/io.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "debug.h"

#define CHANNEL 1
#define RXADDRESS 84
#define PACKET_LENGTH 10

char buffer[PACKET_LENGTH] = {0,0,0,0,0,0,0,0,0,0};

void comm_init() {
  m_bus_init();
  if (!m_rf_open(CHANNEL, RXADDRESS, PACKET_LENGTH)) {
    m_red(ON);
  }
}

void comm_handler() {
  m_rf_read(buffer,PACKET_LENGTH);
  send_hex("buf[0]", buffer[0]);
}
