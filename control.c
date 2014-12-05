#include "debug.h"
#include "localization.h"
#include "motor.h"
#include "adc.h"
#include "timer.h"

#define RIGHT 1
#define LEFT 2

#define BETA 0
#define Kp 50
#define Kd 2
#define Ki 0

#define PI 3.145

POINT *goal = NULL;
int goal_direction;
POINT *robot_goal[2];
int count = 0;
float cf_theta_diff = 0;
float prev_cf_theta_diff = 0;

float translate_theta(float alpha, float beta) {
  float theta = alpha - beta;
  if (theta > PI) {
    theta = -2*PI + theta;
  }
  return theta;
}

void drive_to_goal(POINT *robot) {
  float theta_diff;
  float tgt_duty_cycle_L = 130;
  float tgt_duty_cycle_R = 130;

  robot_goal[0] = robot;
  robot_goal[1] = goal;
  goal->theta = determine_angle(robot_goal);

  theta_diff = robot->theta - goal->theta;
  /* theta_diff = translate_theta(robot->theta, goal->theta); */
  if (cf_theta_diff == 0) {
    cf_theta_diff = theta_diff;
    prev_cf_theta_diff = theta_diff;
  }
  cf_theta_diff = BETA * cf_theta_diff + (1-BETA) * theta_diff;

  tgt_duty_cycle_L -= Kp * cf_theta_diff;
  tgt_duty_cycle_L -= Kd * (cf_theta_diff - prev_cf_theta_diff)/DT;
  tgt_duty_cycle_R += Kp * cf_theta_diff;
  tgt_duty_cycle_R += Kd * (cf_theta_diff - prev_cf_theta_diff)/DT;

  set_motor_duty_cycle((int)tgt_duty_cycle_L, (int)tgt_duty_cycle_R);
  prev_cf_theta_diff = cf_theta_diff;

  send_float("robot->theta", robot->theta * 57.3);
  send_float("goal->theta", goal->theta * 57.3);
  send_float("cf_theta_diff", cf_theta_diff * 57.3);
  send_float("tgt_duty_cycle_L", tgt_duty_cycle_L);
  send_float("tgt_duty_cycle_R", tgt_duty_cycle_R);
}

int determine_goal(POINT *robot) {
  if (robot->x != 0 && (goal == NULL)) {
    if (robot->x < 500) {
      m_green(ON);
      goal = create_point(850, 350);
      goal_direction = RIGHT;
    } else {
      m_red(ON);
      goal = create_point(150, 300);
      goal_direction = LEFT;
    }
  } else {
    count++;
    send_float("goal_direction", goal_direction);
  }

  if (goal != NULL) {
    return 1;
  } else {
    return 0;
  }
}

/* void estimate_puck_theta() { */
/*   int theta_est; */

/*   int sensor_b_l = sensor_values[2]; */
/*   int sensor_t_l = sensor_values[1]; */
/*   int sensor_middle = sensor_values[0]; */
/*   int sensor_t_r = sensor_values[4]; */
/*   int sensor_b_r = sensor_values[3]; */

/*   if (sensor_t_r > sensor_t_l && sensor_t_r > sensor_b_l && */
/*       sensor_t_r > sensor_b_r) { */
/*     m_usb_tx_string("TOP RIGHT QUADRANT.\n"); */
/*   } else if (sensor_b_r > sensor_t_l && sensor_b_r > sensor_b_l) { */
/*     m_usb_tx_string("BOTTOM RIGHT QUADRANT.\n"); */
/*   } else if (sensor_b_l > sensor_t_l) { */
/*     m_usb_tx_string("BOTTOM LEFT QUADRANT.\n"); */
/*   } else { */
/*     m_usb_tx_string("TOP LEFT QUADRANT.\n"); */
/*   } */
/* } */
