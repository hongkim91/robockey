#include "features.h"
#include "control.h"
#include "goalie.h"
#include "localization.h"
#include "motor.h"
#include "adc.h"
#include "debug.h"

bool first_move = TRUE;
int prev_goalie_theta_est = 0;

void track_puck() {
  float KP = 1;
  float KD = 0;

  if (!TRACK_PUCK) return;

  int goalie_theta_est = estimate_track_puck_theta();
  if (first_move) {
    prev_goalie_theta_est = goalie_theta_est;
    first_move = FALSE;
  }

  int speed = KP * goalie_theta_est - KD * (goalie_theta_est - prev_goalie_theta_est);
  prev_goalie_theta_est = goalie_theta_est;

  set_motor_duty_cycle(-speed, -speed);

  m_usb_tx_string("--------------------TRACK PUCK --------------------\n");
  print_raw_sensor_values();
  print_sensor_values();
  send_float("goalie_theta_est", goalie_theta_est);
  send_float("speed", speed);
  send_float("tgt_duty_cycle_R", -speed);
  send_float("tgt_duty_cycle_L", -speed);
}

int estimate_track_puck_theta() {
  int sensor_b_l = sensor_values[2];
  int sensor_t_l = sensor_values[1];
  int sensor_t_r = sensor_values[4];
  int sensor_b_r = sensor_values[3];

  switch (determine_quadrant()) {
  case T_R:
    return sensor_b_r - sensor_t_r;
  case B_R:
    return sensor_b_r - sensor_t_r;
  case B_L:
    return sensor_t_l + sensor_b_l;
  case T_L:
    return -(sensor_t_l + sensor_b_l);
  }
  return 0; // shouldn't happen.
}
