#ifndef comm__
#define comm__

void comm_init(int rx_addr);
void comm_handler();
void send_camera_data(unsigned int* blobs);

#endif
