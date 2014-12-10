#include "constants.h"

void init_constants(int robot_name) {
  if (robot_name == UGLY) {
    RXADDRESS = 84;
    GOAL_KP = 40;
    GOAL_KD = 70;
    GOAL_SPEED_LIMIT = 190;
    PUCK_SPEED_LIMIT = 190;
    PUCK_KP = .1;
    PUCK_KD = 1;
    CENTER_X = 512;
    CENTER_Y = 450;
  } else if (robot_name == HOPE) {
    RXADDRESS = 85;
    GOAL_KP = 40;
    GOAL_KD = 70;
    GOAL_SPEED_LIMIT = 190;
    PUCK_SPEED_LIMIT = 190;
    PUCK_KP = .1;
    PUCK_KD = 1;
    CENTER_X = 512;
    CENTER_Y = 450;
  }
}
