#ifndef motor__
#define motor__

#define CLOCKWISE 0
#define CCLOCKWISE 1

void motor_init();
void rotate(int direction);
void stop();
void go();
void go_forward();

#endif
