#ifndef localization__
#define localization__

#include <stdlib.h>

typedef struct point {
  int x;
  int y;
  float theta;
} POINT;

typedef struct fpoint {
  float x;
  float y;
  float theta;
} FPOINT;

// data parsing
void getData(unsigned int *data, char *line);
void parsePoints(unsigned int *data, POINT **points);

// localization steps
POINT *determine_position(unsigned int *data);
int find_axial_points(POINT **points, POINT **axial_points);
int find_axis_direction(POINT **axial_points, POINT *p3);
float determine_angle(POINT **axial_points);
FPOINT *rotate_point(POINT *p, float theta);
FPOINT *mid_point(FPOINT *p1, FPOINT *p2);

// helper functions
POINT* create_point(int x, int y);
FPOINT *create_fpoint(float x, float y);
POINT *find_third_point(POINT **points, POINT **axial_points);
int num_points(POINT **points);
float distance(POINT *p1, POINT *p2);
float max(float *arr, int length);
long dot_product(long *v1, long* v2);
float magnitude(long *v);

//debugging related
void rotate_blobs(unsigned int *blobs, float theta);

#endif
