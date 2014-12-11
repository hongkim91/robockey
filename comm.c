#include <avr/io.h>
#include <stdio.h>
#include "m_general.h"
#include "m_bus.h"
#include "m_rf.h"
#include "debug.h"
#include "string.h"
#include "control.h"
#include "comm.h"
#include "features.h"

#define CHANNEL 1
#define PACKET_LENGTH 10
#define TXADDRESS 100

char buffer[PACKET_LENGTH] = {0,0,0,0,0,0,0,0,0,0};
char buf[100];
bool play = FALSE;

void comm_init(int rx_addr) {
  m_bus_init();
  if (!m_rf_open(CHANNEL, rx_addr, PACKET_LENGTH)) {
    /* m_red(ON); */
  }
}

void comm_handler() {
  m_rf_read(buffer,PACKET_LENGTH);
  send_hex("buffer[0]", buffer[0]);
  int direction = get_goal_direction();

  if (buffer[0] == ((char) 0xA0)) {
    m_usb_tx_string("Comm Test.\n");
    if (direction == RIGHT) {
      m_red(TOGGLE);
      bi_color_red(TOGGLE);
    } else if (direction == LEFT) {
      m_green(TOGGLE);
      bi_color_blue(TOGGLE);
    }
    send_float("goal_direction", direction);
    send_float("play", is_play());
  } else if (buffer[0] == ((char) 0xA1)) {
    m_usb_tx_string("Play.\n");
    if (direction == RIGHT) {
      m_red(ON);
      bi_color_red(ON);
      m_green(OFF);
      bi_color_blue(OFF);
    } else if (direction == LEFT) {
      m_red(OFF);
      bi_color_red(OFF);
      m_green(ON);
      bi_color_blue(ON);
    }
    play = TRUE;
    send_float("play", is_play());
  } else if (buffer[0] == ((char) 0xA4)) {
    m_usb_tx_string("Pause.\n");
    play = FALSE;
    send_float("play", is_play());
  } else if (buffer[0] == ((char) 0xA6)) {
    m_usb_tx_string("Halftime.\n");
    play = FALSE;
  } else if (buffer[0] == ((char) 0xA7)) {
    m_usb_tx_string("Game Over.\n");
    play = FALSE;
  }
}

bool is_play() {
  if (REQUIRE_COMM) {
    return play;
  } else {
    return TRUE;
  }
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
