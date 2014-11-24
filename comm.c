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
  send_hex("buf[0]", buffer[0]);
}

void receiver_handler() {
  m_rf_read(buffer,PACKET_LENGTH);
  /* sprintf(buf, "[%d,%d]/[%d,%d]/[%d,%d]/[%d,%d]\n", */
  /*         buffer[0], buffer[1], buffer[2], buffer[3], */
  /*         buffer[4], buffer[5], buffer[6], buffer[7]); */
  /* send_buf(buf); */

  /* float *b = (float *) buffer; */
  /* sprintf(buf, "x: [%.3f], y: [%.3f]\n", b[0], b[1]); */
  /* send_buf(buf); */

  /* int x = buffer; */
  /* send_float("x[0]", x); */
}

void send_camera_data(unsigned int* blobs, float x, float y) {
  /* int i, j=0; */
  /* for (i=0; i<12; i=i+3) { */
  /*   memcpy((buffer+j), (blobs+i), 2); */
  /*   buffer[j] = blobs[i]; */
  /*   /\* buffer[j+1] = blobs[i]; *\/ */
  /*   /\* buffer[j+1] = (char) (signed) blobs[i+1]; *\/ */
  /*   j = j+2; */
  /* } */
  memcpy(buffer, &x, 4);
  memcpy(buffer+4, &y, 4);
  m_rf_send(TXADDRESS, buffer, PACKET_LENGTH);
}

void live_plot_data() {
  /* //write data as concatenated hex:  i.e. f0f1f4f5 */
  /* int i; */
  /* for (i = 0 ; i < 9 ; i++){ */
  /*   m_usb_tx_int(buffer[i]); */
  /*   m_usb_tx_char('\t'); */
  /* } */
  /* m_usb_tx_char('\n');  //MATLAB serial command reads 1 line at a time */

  float *b = (float *) buffer;
  sprintf(buf, "%.3f \t %.3f\n", b[0], b[1]);
  send_buf(buf);
}
