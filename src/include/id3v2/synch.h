
#ifndef ID3V2_SYNCH_H
#define ID3V2_SYNCH_H

#include <stdint.h>

uint32_t int_decode(unsigned char *bytes, int size, int offset);
unsigned char *int_encode(uint32_t integer);
uint32_t synch_encode(uint32_t value);
uint32_t synch_decode(uint32_t value);

#endif
