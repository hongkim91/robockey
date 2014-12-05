#include <stdio.h>
#include "m_general.h"
#include "m_wii.h"
#include "debug.h"
#include "localization.h"
#include "control.h"

char buf[100];
bool goal_set = FALSE;

void camera_init() {
  if (!m_wii_open()) {
    m_red(ON);
  }
}

void camera_handler(unsigned int *blobs) {
  if (m_wii_read(blobs)) {
    send_blobs(blobs);
  } else {
    m_usb_tx_string("failed reading from mWii\n");
  }

  POINT *robot = determine_position(blobs);
  sprintf(buf, "x: %d, y: %d\n", robot->x, robot->y);
  send_buf(buf);

  drive_to_goal(robot);
  free(robot);
}

void set_goal(unsigned int *blobs) {
  if (goal_set == TRUE) {
    return;
  }

  if (m_wii_read(blobs)) {
    send_blobs(blobs);
  } else {
    m_usb_tx_string("failed reading from mWii\n");
  }

  POINT *robot = determine_position(blobs);
  goal_set = determine_goal(robot);
  free(robot);
}
