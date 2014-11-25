#ifndef localization__
#define localization__

#include <stdlib.h>

typedef struct point {
  int x;
  int y;
} POINT;

typedef struct fpoint {
  float x;
  float y;
  float theta;
} FPOINT;

FPOINT *determine_position(unsigned int *data);
float determine_angle(POINT **axial_points);
POINT* create_point(int x, int y);
FPOINT *create_fpoint(float x, float y);

#endif
