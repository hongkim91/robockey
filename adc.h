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
void print_raw_values ();
void print_filtered_values();
void filter_sensor_values();


int F0_val;
int F1_val;
int F4_val;
int F5_val;
int F6_val;

int filtered_index;
int filtered_sensor_values[5];

#endif