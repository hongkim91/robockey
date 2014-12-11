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
    WT_KP_H = 8;
    WT_KP_L = 4;
    WT_KD_H = 3;
    WT_KD_L = 3;
  } else if (robot_name == HOPE) {
    RXADDRESS = 85;
    GOAL_KP = 40;
    GOAL_KD = 70;
    GOAL_SPEED_LIMIT = 255;
    PUCK_SPEED_LIMIT = 255;
    PUCK_KP = .1;
    PUCK_KD = 1;
    CENTER_X = 523;
    CENTER_Y = 423;
    TOP_WALL_Y_CUTOFF = 570;
    BOTTOM_WALL_Y_CUTOFF = 305;
    WT_KP_H = 9;
    WT_KP_L = 5;
    WT_KD_H = 3;
    WT_KD_L = 3;
  } else if (robot_name == GENERAL) {
    RXADDRESS = 86;
    GOAL_KP = 120;
    GOAL_KD = 70;
    GOAL_SPEED_LIMIT = 255;
    PUCK_SPEED_LIMIT = 255;
    PUCK_KP = .2;
    PUCK_KD = 2;
    // TODO: FIGURE THIS OUT
    CENTER_X = 523;
    CENTER_Y = 428;
    TOP_WALL_Y_CUTOFF = 570;
    BOTTOM_WALL_Y_CUTOFF = 305;
    WT_KP_H = 9;
    WT_KP_L = 5;
    WT_KD_H = 3;
    WT_KD_L = 3;
  }
}
