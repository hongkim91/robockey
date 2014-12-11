#ifndef comm__
#define comm__

void comm_init();
void comm_reopen();
void comm_handler();
void send_camera_data(unsigned int* blobs, float x, float y);
void live_plot_data();
void receiver_handler();
bool is_play();

#endif
