#include "id3v2/unicode.h"
#include <stdlib.h>
#include <string.h>


void utf16_to_ascii(const char *buf, size_t buf_size, char **out, size_t *out_size) {
  // create output
  *out_size = (buf_size / 2);
  char *o = (char *)malloc(sizeof(char) * (*out_size));
  memset(o, 0, *out_size);

  // output byte index
  size_t cidx = 0;

  for(size_t i=3; i < buf_size; i++) {
    if (buf[i] == 0x00) {
      continue;
    }
    o[cidx] = buf[i];
    cidx++;
  }

  // swap address with given buffer
  *out = o;

}
