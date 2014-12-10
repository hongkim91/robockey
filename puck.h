#ifndef puck__
#define puck__

bool have_puck();
void find_puck(POINT *robot);
void set_motor_duty_cycle(int refA, int refB);
int puck_speed(int theta_est);
int puck_turn(int theta_est, POINT *robot);

#endif
