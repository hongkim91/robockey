#include <math.h>
#include "constants.h"
#include "features.h"
#include "m_general.h"
#include "motor.h"
#include "adc.h"
#include "debug.h"
#include "control.h"
#include "puck.h"

bool first_run = FALSE;
int prev_theta_est = 0;
int theta_est_cutoff = 0;

bool have_puck() {
  if (TEST_PUCK_SENSOR) {
    send_float("puck_sensor", sensor_values[5]);
  }
  // SENSOR ON INDEX 5 IS ASSUMED TOPBOARD.
  if (sensor_values[5] >= 570) {
    return TRUE;
  } else {
    return FALSE;
  }
}

void find_puck(POINT *robot) {
  if (!FIND_PUCK) return;

  int theta_est = estimate_puck_theta();
  if (!first_run) {
    prev_theta_est = theta_est;
    theta_est_cutoff = 1000;
    first_run = TRUE;
  }

  /* int turn = PUCK_KP * theta_est + PUCK_KD *(theta_est - prev_theta_est); */
  int turn = puck_turn(theta_est, robot);
  prev_theta_est = theta_est;
  int speed_val = puck_speed(theta_est);
  if (speed_val > PUCK_SPEED_LIMIT - abs(turn)) {
    speed_val = PUCK_SPEED_LIMIT - abs(turn);
  }

  int tgt_duty_cycle_L = speed_val + turn;
  int tgt_duty_cycle_R = speed_val - turn;

  set_motor_duty_cycle(tgt_duty_cycle_L, tgt_duty_cycle_R);

  if (!TEST_LOCALIZATION_CENTER) {
    m_usb_tx_string("--------------------FIND PUCK --------------------\n");
    print_raw_sensor_values();
    print_sensor_values();
    if (robot != NULL) {
      send_float("robot->theta", robot->theta * 57.3);
    }
    send_float("theta_est", theta_est);
    send_float("turn", turn);
    send_float("speed", speed_val);
    send_float("tgt_duty_cycle_R", tgt_duty_cycle_R);
    send_float("tgt_duty_cycle_L", tgt_duty_cycle_L);
  }
}

int puck_turn(int theta_est, POINT *robot) {
  int sensor_b_l = sensor_values[2];
  int sensor_t_l = sensor_values[1];
  int sensor_t_r = sensor_values[4];
  int sensor_b_r = sensor_values[3];

  float KP = PUCK_KP;
  float KD = PUCK_KD;

  if (wall_trouble(robot)) {
    switch (determine_quadrant()) {
    case T_R:
      if (sensor_t_l < sensor_b_r) {
        KP = 8 * PUCK_KP;
        KD = 3 * PUCK_KD;
      } else {
        KP = 4 * PUCK_KP;
        KD = 3 * PUCK_KD;
      }
      break;
    case T_L:
      if (sensor_t_r < sensor_b_l) {
        KP = 8 * PUCK_KP;
        KD = 3 * PUCK_KD;
      } else {
        KP = 4 * PUCK_KP;
        KD = 3 * PUCK_KD;
      }
      break;
    }
  }
  return KP * theta_est + KD * (theta_est - prev_theta_est);
}

int puck_speed(int theta_est) {
  int sensor_b_l = sensor_values[2];
  int sensor_t_l = sensor_values[1];
  int sensor_t_r = sensor_values[4];
  int sensor_b_r = sensor_values[3];

  switch (determine_quadrant()) {
  case T_R:
    if (sensor_t_l > sensor_b_r) {
      return (PUCK_SPEED_LIMIT -
              (PUCK_SPEED_LIMIT/(float) abs(theta_est_cutoff)) * theta_est);
    } else {
      theta_est_cutoff = theta_est;
      return 0;
    }
  case T_L:
    if (sensor_t_r > sensor_b_l) {
      return (PUCK_SPEED_LIMIT +
              (PUCK_SPEED_LIMIT/(float) abs(theta_est_cutoff)) * theta_est);
    } else {
      theta_est_cutoff = theta_est;
      return 0;
    }
  case B_R:
  case B_L:
    return 0;
  }
  return 0; // shouldn't happen.
}
