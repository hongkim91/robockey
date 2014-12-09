#ifndef motor__
#define motor__

#define CLOCKWISE 0
#define CCLOCKWISE 1

void motor_init();
void rotate(int direction);
void stop();
bool have_puck();
void find_puck();
void set_motor_duty_cycle(int refA, int refB);
int speed(int theta_est);

#endif
