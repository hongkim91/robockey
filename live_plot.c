#include <avr/io.h>
#include "m_general.h"
#include "m_usb.h"

int data[10] = {837,444,880,412,919,440,861,509,0,0};
int i; //variable for for loop

int main(void) {
  m_clockdivide(0);
  m_usb_init();

  char rx_buffer; //computer interactions

  while(1) {
    while(!m_usb_rx_available());  	//wait for an indication from the computer
    rx_buffer = m_usb_rx_char();  	//grab the computer packet

    m_usb_rx_flush();  		//clear buffer

    if(rx_buffer == 1) {
      //write data as concatenated hex:  i.e. f0f1f4f5
      for (i = 0 ; i < 9 ; i++){
        m_usb_tx_int(data[i]);
        m_usb_tx_char('\t');
      }
      m_usb_tx_char('\n');  //MATLAB serial command reads 1 line at a time
    }
  }
  return 0;   //never reached
}
