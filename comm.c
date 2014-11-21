#include <avr/io.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"

#define CHANNEL 1
#define RXADDRESS 84
#define PACKET_LENGTH 10

char buffer[PACKET_LENGTH] = {0,0,0,0,0,0,0,0,0,0};

/* volatile bool new_packet_flag = false; */

void comm_init() {
  m_bus_init(); // enable mBUS
  m_rf_open(CHANNEL, RXADDRESS, PACKET_LENGTH); // configure mRF
}

ISR(INT2_vect){
  m_rf_read(buffer,PACKET_LENGTH);
  /* new_packet_flag = true; */
  m_green(TOGGLE);
}
