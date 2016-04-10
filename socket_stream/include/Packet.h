#ifndef PACKET_H
#define PACKET_H

static const int NUMBER_OF_MOTORS = 2;
static const int PACKET_SIZE = NUMBER_OF_MOTORS*2*sizeof(int);
// format:
// [ id | pos | id | pos ]
typedef unsigned int dxl_comm_packet[2*NUMBER_OF_MOTORS]; 





















#endif // PACKET_H defined
