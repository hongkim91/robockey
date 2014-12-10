#include "constants.h"
#include "features.h"
#include "localization.h"
#include "debug.h"
#include "motor.h"
#include "adc.h"
#include "timer.h"
#include "control.h"
#include "puck.h"

#define PI 3.145

POINT *goal = NULL;
POINT *robot_goal[2];
float prev_theta_diff = 0;
int goal_direction = 0;

void drive_to_goal(POINT *robot) {
  if (!FIND_GOAL) return;

  robot_goal[0] = robot;
  robot_goal[1] = goal;
  goal->theta = determine_angle(robot_goal);

  float theta_diff = translate_theta(robot->theta, goal->theta);
  if (prev_theta_diff == 0) {
    prev_theta_diff = theta_diff;
  }

  int turn = GOAL_KP * theta_diff + GOAL_KD *(theta_diff - prev_theta_diff);
  prev_theta_diff = theta_diff;
  int speed_val = goal_speed(theta_diff);
  if (speed_val > GOAL_SPEED_LIMIT - abs(turn)) {
    speed_val = GOAL_SPEED_LIMIT - abs(turn);
  }

  int tgt_duty_cycle_L = speed_val - turn;
  int tgt_duty_cycle_R = speed_val + turn;

  m_usb_tx_string("--------------------FIND GOAL--------------------\n");
  set_motor_duty_cycle(tgt_duty_cycle_L, tgt_duty_cycle_R);

  send_float("robot->theta", robot->theta * 57.3);
  send_float("goal->theta", goal->theta * 57.3);
  send_float("theta_diff", theta_diff * 57.3);
  send_float("turn", turn);
  send_float("speed_val", speed_val);
  send_float("speed", goal_speed(theta_diff));

  send_float("tgt_duty_cycle_L", tgt_duty_cycle_L);
  send_float("tgt_duty_cycle_R", tgt_duty_cycle_R);
}

int goal_speed(float theta) {
  float theta_cutoff = 45.0 * PI/180;

  if (0 < theta && theta <= theta_cutoff) {
    return GOAL_SPEED_LIMIT - (GOAL_SPEED_LIMIT/fabs(theta_cutoff)) * theta;
  } else if (-theta_cutoff < theta && theta <= 0) {
    return GOAL_SPEED_LIMIT + (GOAL_SPEED_LIMIT/fabs(theta_cutoff)) * theta;
  } else {
    return 0;
  }
}

int determine_goal(POINT *robot) {
  if (goal == NULL) {
    if (robot->x < 500) {
      /* m_green(ON); */
      goal = create_point(860, 420);
      goal_direction = RIGHT;
      m_usb_tx_string("goal direction: RIGHT\n");
    } else {
      /* m_red(ON); */
      goal = create_point(100, 450);
      goal_direction = LEFT;
      m_usb_tx_string("goal direction: LEFT\n");
    }
  }

  if (goal != NULL) {
    return 1;
  } else {
    return 0;
  }
}

int determine_quadrant() {
  int sensor_b_l = sensor_values[2];
  int sensor_t_l = sensor_values[1];
  int sensor_t_r = sensor_values[4];
  int sensor_b_r = sensor_values[3];

  if (sensor_t_r > sensor_t_l && sensor_t_r > sensor_b_l &&
      sensor_t_r > sensor_b_r) {
    /* m_usb_tx_string("TOP RIGHT QUADRANT.\n"); */
    return T_R;
  } else if (sensor_b_r > sensor_t_l && sensor_b_r > sensor_b_l) {
    /* m_usb_tx_string("BOTTOM RIGHT QUADRANT.\n"); */
    return B_R;
  } else if (sensor_b_l > sensor_t_l) {
    /* m_usb_tx_string("BOTTOM LEFT QUADRANT.\n"); */
    return B_L;
  } else {
    /* m_usb_tx_string("TOP LEFT QUADRANT.\n"); */
    return T_L;
  }
}

int estimate_puck_theta() {
  int sensor_b_l = sensor_values[2];
  int sensor_t_l = sensor_values[1];
  int sensor_t_r = sensor_values[4];
  int sensor_b_r = sensor_values[3];

  switch (determine_quadrant()) {
  case T_R:
    return sensor_t_r - sensor_t_l;
  case B_R:
    return sensor_b_r + sensor_b_l;
  case B_L:
    return -(sensor_b_r + sensor_b_l);
  case T_L:
    return sensor_t_r - sensor_t_l;
  }
  return 0; // shouldn't happen.
}

float translate_theta(float alpha, float beta) {
  float theta = alpha - beta;
  if (theta > PI) {
    theta = -2*PI + theta;
  } else if (theta < -PI) {
    theta = 2*PI + theta;
  }
  return theta;
}

int get_goal_direction() {
  return goal_direction;
}

void bi_color_red(int mode) {
  if (mode == ON) {
    set(PORTB, 2);
  } else if (mode == OFF) {
    clear(PORTB, 2);
  } else if (mode == TOGGLE) {
    toggle(PORTB, 2);
  }
}

void bi_color_blue(int mode) {
  if (mode == ON) {
    set(PORTB, 3);
  } else if (mode == OFF) {
    clear(PORTB, 3);
  } else if (mode == TOGGLE) {
    toggle(PORTB, 3);
  }
}

void light_up_with_puck() {
  if (have_puck()) {
    bi_color_red(ON);
  } else {
    bi_color_red(OFF);
  }
}
