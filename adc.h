#ifndef adc__
#define adc__

void init();
void init_ADC ();
void update_ADC ();
void ADC_F0 ();
void ADC_F1 ();
void ADC_F4 ();
void ADC_F5 ();
void ADC_F6 ();
void matlab();
void print_sensor_values();
void filter_sensor_values();

int sensor_values[5];

#endif
