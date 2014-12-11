#include "constants.h"

void init_constants(int robot_name) {
  if (robot_name == UGLY) {
    RXADDRESS = 84;
    GOAL_KP = 40;
    GOAL_KD = 70;
    GOAL_SPEED_LIMIT = 255;
    PUCK_SPEED_LIMIT = 255;
    PUCK_KP = .1;
    PUCK_KD = 1;
    CENTER_X = 512;
    CENTER_Y = 450;
    TOP_WALL_Y_CUTOFF = 570;
    BOTTOM_WALL_Y_CUTOFF = 305;
  } else if (robot_name == HOPE) {
    RXADDRESS = 85;
    GOAL_KP = 40;
    GOAL_KD = 70;
    GOAL_SPEED_LIMIT = 255;
    PUCK_SPEED_LIMIT = 255;
    PUCK_KP = .1;
    PUCK_KD = 1;
    CENTER_X = 523;
    CENTER_Y = 427;
    TOP_WALL_Y_CUTOFF = 570;
    BOTTOM_WALL_Y_CUTOFF = 305;
  }
}
