#include <stdio.h>
#include "m_general.h"
#include "m_wii.h"
#include "debug.h"
#include "localization.h"
#include "motor.h"

#define NOTSET 0
#define RIGHT 1
#define LEFT 2

#define TOLERANCE 0.2

char buf[100];
POINT *robot_goal[2];
int goal = NOTSET;
int count = 0;

int determine_goal();

void camera_init() {
  if (!m_wii_open()) {
    m_red(ON);
  }
}

void camera_handler(unsigned int *blobs) {
  float goal_theta;

  if (m_wii_read(blobs)) {
    sprintf(buf, "[%d,%d]/[%d,%d]/[%d,%d]/[%d,%d]\n",
            blobs[0], blobs[1], blobs[3], blobs[4],
            blobs[6], blobs[7], blobs[9], blobs[10]);
    send_buf(buf);
  } else {
    m_usb_tx_string("failed reading from mWii");
  }

  POINT *robot = determine_position(blobs);
  send_float("x", robot->x);
  send_float("y", robot->y);
  send_float("theta", robot->theta * 57.3);

  /* if (!determine_goal(robot)) { */
  /*   return; */
  /* } */

  if (robot->x > 850) {
    stop();
    return;
  }

  robot_goal[0] = robot;
  robot_goal[1] = create_point(820, 350);
  goal_theta = determine_angle(robot_goal);

  if (fabs(robot->theta - goal_theta) < TOLERANCE) {
    m_red(ON);
    m_green(OFF);
    /* stop(); */
    set_motor_voltage(255);
    go_forward();
  } else if ((robot->theta - goal_theta) > TOLERANCE)  {
    m_red(OFF);
    m_green(ON);
    set_motor_voltage(50);
    rotate(CLOCKWISE);
    go();
  } else {
    m_red(OFF);
    m_green(ON);
    set_motor_voltage(50);
    rotate(CCLOCKWISE);
    go();
  }

  /* send_camera_data(blobs, robot->theta*57.3, goal_theta*57.3); */

  /* send_float("robot->theta", robot->theta * 57.3); */
  /* send_float("goal_theta", goal_theta * 57.3); */
  free(robot_goal[0]);

  /* send_camera_data(blobs, robot->x, robot->y); */
}

int determine_goal(POINT *robot) {
  if (!goal && count > 60) {
    if (robot->x > 500) {
      /* send_point(x,y); */
      /* m_red(ON); */
      goal = RIGHT;
      robot_goal[1] = create_point(820, 350);
    } else {
      /* send_point(x,y); */
      /* m_green(ON); */
      goal = LEFT;
      robot_goal[1] = create_point(200, 350);
    }
  } else {
    count++;
  }

  if (goal) {
    return 1;
  } else {
    return 0;
  }
}
