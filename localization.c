#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "localization.h"
#include "debug.h"
#include "features.h"

#define RINK_PIXEL_WIDTH 1024   // 512
#define RINK_PIXEL_HEIGHT 768   // 384
#define CENTER_X 512
#define CENTER_Y 450

int initialized = 0;
POINT *axial_points[2];
POINT *points[4];

/*
 * DATA PARSING
 */
void getData(unsigned int *data, char *line) {
  char* tok;
  int i;
  for (i = 0; i < 12; i++) {
    if (i == 0) {
      tok = strtok(line, ",");
    } else {
      tok = strtok(NULL, ",");
    }
    data[i] = atoi(tok);
  }
}

void parsePoints(unsigned int *data, POINT **points) {
  int i;
  int p_idx = 0;
  for (i=0; i<12; i=i+3) {
    if (data[i] != 1023) {
      if (points[p_idx] == NULL) {
        points[p_idx] = create_point(data[i], data[i+1]);
      } else {
        points[p_idx]->x = data[i];
        points[p_idx]->y = data[i+1];
      }
      p_idx++;
    }
  }
  for (; p_idx < 4; p_idx++) {
    if (points[p_idx] != NULL) {
      free(points[p_idx]);
    }
    points[p_idx] = NULL;
  }
}

/*
 * LOCALIATION STEPS to determine robot position
 * 1. Find axial points.
 * 2. Perform triangular analysis to finalize axis direction.
 * 3. Calculate theta orientation.
 * 4. Rotate the axis by theta.
 * 5. Calculate mid point of the axis points.
 */
POINT *determine_position(unsigned int *data) {
  float theta;

  if (!initialized) {
    int i;
    for (i=0; i<2; i++) {
      axial_points[i] = NULL;
    }
    for (i=0; i<4; i++) {
      points[i] = NULL;
    }
    initialized = 1;
  }
  parsePoints(data, points);

  if (!find_axial_points(points, axial_points)) {
    return NULL;
  }
  if (!find_axis_direction(axial_points,
                           find_third_point(points, axial_points))) {
    return NULL;
  }
  theta = determine_angle(axial_points);

  if (TEST_LOCALIZATION_CENTER) {
    while(!m_usb_rx_available()); //wait for an indication from the computer
    m_usb_rx_char();  	          //grab the computer packet
    m_usb_rx_flush();  		  //clear buffer
    POINT *p1 = axial_points[0];
    POINT *p2 = axial_points[0];
    send_point((p1->x+p2->x)/2, (p1->y+p1->y)/2);
  }

  FPOINT *rp1 = rotate_point(axial_points[0], theta);
  FPOINT *rp2 = rotate_point(axial_points[1], theta);
  FPOINT *mp = mid_point(rp1, rp2);

  // The camera's coordinate system has its origin in the top-right.
  POINT *pos = create_point(RINK_PIXEL_WIDTH - mp->x, mp->y);
  pos->theta = theta;

  free(rp1);
  free(rp2);
  free(mp);
  return pos;
}

int find_axial_points(POINT **points, POINT **axial_points) {
  int n = num_points(points);
  POINT *p1 = points[0];
  POINT *p2 = points[1];
  POINT *p3 = points[2];
  POINT *p4 = points[3];

  if (n == 4) {
    float d1 = distance(p1, p2);
    float d2 = distance(p1, p3);
    float d3 = distance(p1, p4);
    float d4 = distance(p2, p3);
    float d5 = distance(p2, p4);
    float d6 = distance(p3, p4);
    float distances[6] = {d1,d2,d3,d4,d5,d6};
    if (max(distances, 6) == d1) {
      axial_points[0] = p1;
      axial_points[1] = p2;
    } else if (max(distances, 6) == d2) {
      axial_points[0] = p1;
      axial_points[1] = p3;
    } else if (max(distances, 6) == d3) {
      axial_points[0] = p1;
      axial_points[1] = p4;
    } else if (max(distances, 6) == d4) {
      axial_points[0] = p2;
      axial_points[1] = p3;
    } else if (max(distances, 6) == d5) {
      axial_points[0] = p2;
      axial_points[1] = p4;
    } else if (max(distances, 6) == d6) {
      axial_points[0] = p3;
      axial_points[1] = p4;
    }
    return 1;
  } else if (n == 3) {
    float d1 = distance(p1, p2);
    float d2 = distance(p1, p3);
    float d3 = distance(p2, p3);
    float distances[3] = {d1,d2,d3};
    if (max(distances, 3) == d1) {
      axial_points[0] = p1;
      axial_points[1] = p2;
    } else if (max(distances, 3) == d2) {
      axial_points[0] = p1;
      axial_points[1] = p3;
    } else if (max(distances, 3) == d3) {
      axial_points[0] = p2;
      axial_points[1] = p3;
    }
    return 1;
  } else{
    return 0;
    /* printf("LESS THAN THREE STARS!\n"); */
  }
}

int find_axis_direction(POINT **axial_points, POINT *p3) {
  POINT *p1 = axial_points[0];
  POINT *p2 = axial_points[1];
  float d1 = distance(p1, p2);
  float d2 = distance(p2, p3);
  float ratio = d2/d1;

  /* The four ratios d2/d1 can have: */
  /* 16/29     20/29     13/29     26/29 */
  /* 0.5517    0.6897    0.4483    0.8966 */

  if ((ratio > 0.4 && ratio <= 0.5) ||
      (ratio > 0.5 && ratio <= 0.6)) {
    // swap to ensure axial_points[0] is the NORTH star.
    POINT *temp = p1;
    axial_points[0] = p2;
    axial_points[1] = temp;
    return 1;
  } else if ((ratio > 0.64 && ratio <= 0.74) ||
             (ratio > 0.85 && ratio <= 0.95)) {
    // already in correct order.
    return 1;
  } else {
    /* printf("triangle analysis failed to classify\n"); */
    return 0;
  }
}

float determine_angle(POINT **axial_points) {
  POINT *a = axial_points[0];
  POINT *b = axial_points[1];

  long v1[] = {a->x - b->x, a->y - b->y};
  long v2[] = {0, -1};

  float theta = acos(dot_product(v1, v2) / magnitude(v1)*magnitude(v2));
  if (a->x > b->x) {
    theta = -theta;
  }
  return theta;
}

FPOINT *rotate_point(POINT *p, float theta) {
  int dx = (p->x - CENTER_X);
  int dy = (p->y - CENTER_Y);

  FPOINT *rp = create_fpoint(0,0);
  rp->x =  dx * cos(theta) + dy * -sin(theta) + CENTER_X;
  rp->y =  dx * sin(theta) + dy * cos(theta) + CENTER_Y;
  return rp;
}

FPOINT *mid_point(FPOINT *p1, FPOINT *p2) {
  FPOINT *p = create_fpoint(0,0);
  p->x = (p1->x+p2->x)/2;
  p->y = (p1->y+p2->y)/2;
  return p;
}

/*
 * HELPER FUNCTIONS
 */
POINT *create_point(int x, int y) {
  POINT *point = (POINT *) malloc(sizeof(POINT));
  point->x = x;
  point->y = y;
  return point;
}

FPOINT *create_fpoint(float x, float y) {
  FPOINT *fpoint = (FPOINT *) malloc(sizeof(FPOINT));
  fpoint->x = x;
  fpoint->y = y;
  return fpoint;
}

POINT *find_third_point(POINT **points, POINT **axial_points) {
  int i;
  for (i=0; i<4; i++) {
    if (points[i] != axial_points[0] && points[i] != axial_points[1]) {
      return points[i];
    }
  }
  return NULL; // shouldn't happen.
}

int num_points(POINT **points) {
  int n = 0;
  int i;
  for (i=0; i<4; i++) {
    if (points[i] != NULL) {
      n++;
    }
  }
  return n;
}

float distance(POINT *p1, POINT *p2) {
  return sqrt(pow(p1->x-p2->x,2)+pow(p1->y-p2->y,2));
}

float max(float *arr, int length) {
  float max = 0;
  int i;
  for (i=0; i<length; i++) {
    if (arr[i] > max) {
      max = arr[i];
    }
  }
  return max;
}

long dot_product(long *v1, long* v2) {
  return v1[0]*v2[0] + v1[1]*v2[1];
}

float magnitude(long *v) {
  return sqrt(dot_product(v,v));
}

/*
 * DEBUGGING RELATED
 */

void rotate_blobs(unsigned int *blobs, float theta) {
  parsePoints(blobs, points);
  FPOINT *rp1 = rotate_point(points[0], theta);
  FPOINT *rp2 = rotate_point(points[1], theta);
  FPOINT *rp3 = rotate_point(points[2], theta);
  FPOINT *rp4 = rotate_point(points[3], theta);

  blobs[0] = rp1->x;
  blobs[1] = rp1->y;
  blobs[3] = rp2->x;
  blobs[4] = rp2->y;
  blobs[6] = rp3->x;
  blobs[7] = rp3->y;
  blobs[9] = rp4->x;
  blobs[10] = rp4->y;

  free(rp1);
  free(rp2);
  free(rp3);
  free(rp4);
}

// INDEPENDENT LOCALIZATION TESTING
/* int main() */
/* { */
/*   /\* FILE *file = fopen("A.csv", "r"); *\/ */
/*   /\* FILE *file = fopen("B.csv", "r"); *\/ */
/*   FILE *file = fopen("C.csv", "r"); */

/*   unsigned int data[12]; */
/*   char line[128]; */

/*   while (fgets(line, 128, file)) { */
/*     getData(data, line); */

/*     FPOINT *mp = determine_position(data); */
/*     printf("x:%f, y:%f\n", mp->x, mp->y); */
/*     free(mp); */
/*   } */
/* } */

