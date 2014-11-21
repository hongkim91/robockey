#include <string.h>
#include <stdio.h>
#include "m_usb.h"

char buf[100];

void send_str(char *str){
  int i;
  for (i=0; i< strlen(buf); i++) {
    m_usb_tx_char(*(buf+i));
  }
}

void send_float(char *label, float value){
  sprintf(buf, "%s: %.3f\n", label, value);
  send_str(buf);
}
