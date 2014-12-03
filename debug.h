#ifndef debug__
#define debug__

#include "m_usb.h"
#include "localization.h"

void debug_init();
void send_buf(char *buf);
void send_float(char *label, float value);
void send_hex(char *label, char value);
void send_point(int x, int y);
void wait_matlab();
void send_matlab_blobs(unsigned int* blobs);
void send_matlab_robot(POINT* robot);
void send_matlab_localization(unsigned int *blobs);
void send_blobs(unsigned int *blobs);
void send_int(char *label, int x);

#endif
