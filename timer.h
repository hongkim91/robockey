#ifndef timer__
#define timer__

#define SYSTEM_CLOCK 16*pow(10,6)
#define POLLING_FREQ 10
#define INTERVAL_TICK_FREQ 100
#define DT 1.0/POLLING_FREQ

void init_timer0();
void init_timer1();
void init_timer3();
void camera_data_handler(unsigned int *blobs);
void increment_interval_count();
bool interval_timer_running();
void set_interval(unsigned int val);

#endif
