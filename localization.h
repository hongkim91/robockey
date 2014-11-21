#ifndef localization__
#define localization__

typedef struct point {
  int x;
  int y;
} POINT;

typedef struct fpoint {
  float x;
  float y;
} FPOINT;

FPOINT *determine_position(unsigned int *data);
POINT* create_point(int x, int y);
FPOINT *create_fpoint(float x, float y);

#endif
