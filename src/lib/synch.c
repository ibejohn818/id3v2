#include "id3v2/synch.h"
#include <stdlib.h>

uint32_t int_decode(unsigned char *bytes, int size, int offset) {
  unsigned int result = 0x00;
  int i = 0;
  for (i = 0; i < size; i++) {
    result = result << 8;
    result = result | (unsigned char)bytes[offset + i];
  }

  return result;
}

unsigned char *int_encode(uint32_t integer) {
  int i;
  int size = 4;
  unsigned char *result = (unsigned char *)malloc(sizeof(char) * size);

  // We need to reverse the bytes because Intel uses little endian.
  unsigned char *aux = (unsigned char *)&integer;
  for (i = size - 1; i >= 0; i--) {
    result[size - 1 - i] = aux[i];
  }

  return result;
}

uint32_t synch_encode(uint32_t value) {
  int out, mask = 0x7F;

  while (mask ^ 0x7FFFFFFF) {
    out = value & ~mask;
    out <<= 1;
    out |= value & mask;
    mask = ((mask + 1) << 8) - 1;
    value = out;
  }

  return out;
}

uint32_t synch_decode(uint32_t value) {
  unsigned int a, b, c, d, result = 0x0;
  a = value & 0xFF;
  b = (value >> 8) & 0xFF;
  c = (value >> 16) & 0xFF;
  d = (value >> 24) & 0xFF;

  result = result | a;
  result = result | (b << 7);
  result = result | (c << 14);
  result = result | (d << 21);

  return result;
}
