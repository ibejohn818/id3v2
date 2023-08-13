
#ifndef ID3V2_SYNCH_H
#define ID3V2_SYNCH_H

#include <stdint.h>

uint32_t int_decode(char *bytes, int size, int offset);
char *int_encode(int integer);
uint32_t synch_encode(int value);
uint32_t synch_decode(int value);

#endif
