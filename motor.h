#ifndef motor__
#define motor__

#define CLOCKWISE 0
#define CCLOCKWISE 1

int error_l; //error on left motor
int error_r; // error on right motor
int derror_l; // derivative error left
int derror_r; // derivative error right
int c_l; // adjustment left
int c_r; // adjustment right
int refA;
int refB;

int sensor_t_l; // top left sensor value
int sensor_t_r;
int sensor_b_l;
int sensor_b_r;
int sensor_middle;

void motor_init();
void rotate(int direction);
void stop();
void go();
void go_forward();
void find_puck();
int map_value(int input,int in_min, int in_max, int out_min, int out_max);
void set_motor_duty_cycle(int refA, int refB);

#endif
