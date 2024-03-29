#include <stdio.h>
#include "m_general.h"
#include "m_wii.h"
#include "debug.h"
#include "localization.h"
#include "control.h"
#include "features.h"

unsigned int blobs[12];
char buf[100];
bool goal_set = FALSE;
POINT *curr_robot = NULL;

void camera_init() {
  if (!m_wii_open()) {
    m_red(ON);
  }
}

void attempt_localization() {
  if (m_wii_read(blobs)) {
    if (TEST_STAR_READING) {
      send_blobs(blobs);
    }
  } else {
    m_usb_tx_string("failed reading from mWii\n");
  }

  POINT *new_robot = determine_position(blobs);
  if (new_robot != NULL) {
    if (curr_robot != NULL) { // might be the first time curr_robot is set.
      free(curr_robot);
    }
    curr_robot = new_robot;
  }
}

POINT *localize_robot() {
  if (!LOCALIZATION) return NULL;

  attempt_localization();

  if (curr_robot != NULL) {
    if (LOCALIZATION && !TEST_LOCALIZATION_CENTER) {
      sprintf(buf, "x: %d, y: %d\n", curr_robot->x, curr_robot->y);
      send_buf(buf);
    }
    if (TEST_WALL_TROUBLE) {
      if (wall_trouble(curr_robot)) {
        bi_color_red(ON);
      } else {
        bi_color_red(OFF);
      }
    }
  } else {
    if (LOCALIZATION && !TEST_LOCALIZATION_CENTER) {
      m_usb_tx_string("No point for curr_robot has been set yet!\n");
    }
  }
  return curr_robot;
}

bool set_goal() {
  if (goal_set == TRUE) {
    return TRUE;
  }

  if (curr_robot != NULL) {
    goal_set = determine_goal(curr_robot);
  }
  return goal_set;
}
