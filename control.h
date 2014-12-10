#ifndef control__
#define control__

#define NOTSET 0
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
void bi_color_red(int mode);
void bi_color_blue(int mode);
void light_up_with_puck();
bool headed_own_goal(POINT *robot);
bool near_wall(POINT *robot);
bool wall_trouble(POINT *robot);

#endif
