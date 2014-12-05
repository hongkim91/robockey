//
//  main.c
//  IR testing
//
//  Created by Max Wasserman on 11/21/2014.
//
//


#include "m_general.h"
#include "adc.h"
#include "m_usb.h"

#define BETA 0.0
// define variables
char rx_buffer; //computer interactions

int i; //variable for for loop

char rx_buffer;

void init_ADC (void)
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

void update_ADC (void)
{
    //read ADC value from F0
    ADC_F0();
    sensor_values[0] = ADC;

    //read ADC value from F1
    ADC_F1();
    sensor_values[1] = ADC;

    //read ADC value from F4
    ADC_F4();
    sensor_values[2] = ADC;

    //read ADC value from F5
    ADC_F5();
    sensor_values[3] = ADC;

    //read ADC value from F6
    ADC_F6();
    sensor_values[4] = ADC;
}

void ADC_F0 (void)
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

void ADC_F1 (void)
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

void ADC_F4 (void)
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

void ADC_F5(void)
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

void ADC_F6(void)
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

void matlab(void)
{
  while(!m_usb_rx_available()); {     //wait for an indication from the computer
    rx_buffer = m_usb_rx_char();  }  //grab the computer packet

  m_usb_rx_flush();               //clear buffer

  if(rx_buffer == 1) {            //computer wants ir data
    print_sensor_values();
  }
}

void print_sensor_values()
{
    m_usb_tx_int(sensor_values[2]); // back left
    m_usb_tx_string(" ");
    m_usb_tx_int(sensor_values[1]); // top left
    m_usb_tx_string(" ");
    m_usb_tx_int(sensor_values[0]); // middle
    m_usb_tx_string(" ");
    m_usb_tx_int(sensor_values[4]); // top right
    m_usb_tx_string(" ");
    m_usb_tx_int(sensor_values[3]); // back right
    m_usb_tx_string("\n");
}

void offset_sensor_values() {
}
