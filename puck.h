#ifndef puck__
#define puck__

bool have_puck();
void find_puck();
void set_motor_duty_cycle(int refA, int refB);
int puck_speed(int theta_est);

#endif
