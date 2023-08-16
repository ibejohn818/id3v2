
#ifndef ID3V2_SYNCH_H
#define ID3V2_SYNCH_H

#include <stdint.h>

uint32_t int_decode(unsigned char *bytes, int size, int offset);
unsigned char *int_encode(uint32_t integer);
uint32_t synch_encode(int value);
uint32_t synch_decode(int value);

#endif
