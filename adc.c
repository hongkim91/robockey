//
//  main.c
//  IR testing
//
//  Created by Max Wasserman on 11/21/2014.
//
//


#include "m_general.h"
#include "adc.h"
#include "debug.h"
#include "features.h"
#include <stdio.h>

#define BETA 0.0
// define variables
char rx_buffer; //computer interactions

int i; //variable for for loop

char rx_buffer;

void init_ADC()
{
    //set voltage reference of ADC  to AREF
    clear(ADMUX,REFS1);
    set(ADMUX,REFS0);  // set this if you want Vcc

    //ADC prescaler = 128 -->fadc = 125 kHz
    set(ADCSRA,ADPS2);
    set(ADCSRA,ADPS1);
    set(ADCSRA,ADPS0);

    //F0 - 0, ... F7 - 7... D4 - 8,... D7 - 10.... B4 - 11... B6 - 13
    //turn off digital input circuitry for pin F0, F1, F4
    set(DIDR0,ADC0D);
    set(DIDR0,ADC1D);
    set(DIDR0,ADC4D);

    // set pins as inputs
    clear(DDRF,0);
    clear(DDRF,1);
    clear(DDRF,4);
    clear(DDRF,5);
    clear(DDRF,6);

    // turn off pull up resistors for all inputs
    clear(PORTF,0);
    clear(PORTF,1);
    clear(PORTF,4);
    clear(PORTF,5);
    clear(PORTF,6);

    // enable interrupt when ADC is finished
    set(ADCSRA,ADIE);

    //enable free running mode
    set(ADCSRA,ADATE);

    //enable ADC subsystem
    set(ADCSRA,ADEN);

    m_disableJTAG();

}

void update_ADC()
{
    //read ADC value from F0
    ADC_F0();
    raw_sensor_values[0] = ADC;

    //read ADC value from F1
    ADC_F1();
    raw_sensor_values[1] = ADC;

    //read ADC value from F4
    ADC_F4();
    raw_sensor_values[2] = ADC;

    //read ADC value from F5
    ADC_F5();
    raw_sensor_values[3] = ADC;

    //read ADC value from F6
    ADC_F6();
    raw_sensor_values[4] = ADC;

    //read ADC value from F7
    ADC_F7();
    raw_sensor_values[5] = ADC;

    offset_sensor_values();
    if (TEST_SENSORS) {
      print_raw_sensor_values();
    }
}

void ADC_F0()
{ //disable ADC subsystem - ensures good behavior
    clear(ADCSRA,ADEN);

    //enable F0 as input
    clear(ADCSRB,MUX5);
    clear(ADMUX,MUX2);
    clear(ADMUX,MUX1);
    clear(ADMUX, MUX0);

    //enable ADC subsystem
    set(ADCSRA,ADEN);

    //begin conversion
    set(ADCSRA,ADSC);

    //check if flag is set
    while(!check(ADCSRA,ADIF));
}

void ADC_F1()
{ //disable ADC subsystem - ensures good behavior
    clear(ADCSRA,ADEN);

    //enable F1 as input
    clear(ADCSRB,MUX5);
    clear(ADMUX,MUX2);
    clear(ADMUX,MUX1);
    set(ADMUX, MUX0);

    //enable ADC subsystem
    set(ADCSRA,ADEN);
    //begin conversion
    set(ADCSRA,ADSC);

    //check if flag is set
    while(!check(ADCSRA,ADIF));
}

void ADC_F4()
{ //disable ADC subsystem - ensures good behavior
    clear(ADCSRA,ADEN);

    //enable F4 as input
    clear(ADCSRB,MUX5);
    set(ADMUX,MUX2);
    clear(ADMUX,MUX1);
    clear(ADMUX, MUX0);

    //enable ADC subsystem
    set(ADCSRA,ADEN);//begin conversion
    set(ADCSRA,ADSC);

    //check if flag is set
    while(!check(ADCSRA,ADIF));
}

void ADC_F5()
{ //disable ADC subsystem - ensures good behavior
    clear(ADCSRA,ADEN);

    //enable F5 as input
    clear(ADCSRB,MUX5);
    set(ADMUX,MUX2);
    clear(ADMUX,MUX1);
    set(ADMUX, MUX0);

    //enable ADC subsystem
    set(ADCSRA,ADEN);//begin conversion
    set(ADCSRA,ADSC);

    //check if flag is set
    while(!check(ADCSRA,ADIF));
}

void ADC_F6()
{ //disable ADC subsystem - ensures good behavior
    clear(ADCSRA,ADEN);

    //enable F6 as input
    clear(ADCSRB,MUX5);
    set(ADMUX,MUX2);
    set(ADMUX,MUX1);
    clear(ADMUX, MUX0);

    //enable ADC subsystem
    set(ADCSRA,ADEN);//begin conversion
    set(ADCSRA,ADSC);

    //check if flag is set
    while(!check(ADCSRA,ADIF));
}

void ADC_F7()
{ //disable ADC subsystem - ensures good behavior
    clear(ADCSRA,ADEN);

    //enable F7 as input
    clear(ADCSRB,MUX5);
    set(ADMUX,MUX2);
    set(ADMUX,MUX1);
    set(ADMUX, MUX0);

    //enable ADC subsystem
    set(ADCSRA,ADEN);//begin conversion
    set(ADCSRA,ADSC);

    //check if flag is set
    while(!check(ADCSRA,ADIF));
}

void matlab()
{
  while(!m_usb_rx_available()); {     //wait for an indication from the computer
    rx_buffer = m_usb_rx_char();  }  //grab the computer packet

  m_usb_rx_flush();               //clear buffer

  if(rx_buffer == 1) {            //computer wants ir data
    print_sensor_values();
  }
}

char buf[100];
void print_sensor_values()
{
  sprintf(buf, "%3d %3d %3d %3d %3d     %3d\n", sensor_values[2], sensor_values[1],
          sensor_values[0], sensor_values[4], sensor_values[3], sensor_values[5]);
  send_buf(buf);
}

void print_raw_sensor_values()
{
  sprintf(buf, "%3d %3d %3d %3d %3d     %3d\n", raw_sensor_values[2],
          raw_sensor_values[1], raw_sensor_values[0], raw_sensor_values[4],
          raw_sensor_values[3], raw_sensor_values[5]);
  send_buf(buf);
}

void offset_sensor_values() {
  int i;
  int lowest_idx1 = 0;
  int lowest_idx2 = 0;
  int avg_raw_sensor_value;

  for (i=1; i<5; i++) {
    if (lowest_idx1 == 0 ||
        raw_sensor_values[i] < raw_sensor_values[lowest_idx1]) {
      lowest_idx1 = i;
    }
  }
  for (i=1; i<5; i++) {
    if (i == lowest_idx1) continue;
    if (lowest_idx2 == 0 ||
        raw_sensor_values[i] < raw_sensor_values[lowest_idx2]) {
      lowest_idx2 = i;
    }
  }
  avg_raw_sensor_value = (raw_sensor_values[lowest_idx1] +
                      raw_sensor_values[lowest_idx2])/2;

  for (i=1; i<5; i++) {
    sensor_values[i] = raw_sensor_values[i] - avg_raw_sensor_value;
  }
  sensor_values[0] = raw_sensor_values[0];
  sensor_values[5] = raw_sensor_values[5];

}
