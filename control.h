#ifndef control__
#define control__

#define RIGHT 1
#define LEFT 2

#define T_R 1
#define B_R 2
#define B_L 3
#define T_L 4

void drive_to_goal(POINT *robot);
int determine_goal(POINT *robot);
int estimate_puck_theta();
int determine_quadrant();
float translate_theta(float alpha, float beta);
int goal_speed(float theta);
int get_goal_direction();

#endif
