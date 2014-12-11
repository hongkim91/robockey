#ifndef constants__
#define constants__

// robot names.
#define UGLY 1
#define HOPE 2
#define GENERAL 3

int ROBOT_NAME;
int RXADDRESS;
int GOAL_KP;
int GOAL_KD;
float GOAL_SPEED_LIMIT;
float PUCK_SPEED_LIMIT;
float PUCK_KP;
float PUCK_KD;
int CENTER_X;
int CENTER_Y;
int TOP_WALL_Y_CUTOFF;
int BOTTOM_WALL_Y_CUTOFF;

void init_constants();

#endif
