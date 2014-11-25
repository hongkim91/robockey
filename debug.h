#ifndef debug__
#define debug__

#include "m_usb.h"

void debug_init();
void send_buf(char *buf);
void send_float(char *label, float value);
void send_hex(char *label, char value);
void send_point(int x, int y);

#endif
