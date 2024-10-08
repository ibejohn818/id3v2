#include <stdio.h>
#include <stdlib.h>
#include "../include/id3v2/unicode.h"
/*
  test a utf16 hack that only removes 0 byte indexes
*/

static char tb[] = {
    0x01, 0xff, 0xfe, 0x4c, 0x00, 0x6f, 0x00, 0x6f, 0x00, 0x6b, 0x00, 0x20,
    0x00, 0x41, 0x00, 0x74, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x65, 0x00, 0x20,
    0x00, 0x4e, 0x00, 0x6f, 0x00, 0x77, 0x00, 0x20, 0x00, 0x28, 0x00, 0x66,
    0x00, 0x65, 0x00, 0x61, 0x00, 0x74, 0x00, 0x75, 0x00, 0x72, 0x00, 0x69,
    0x00, 0x6e, 0x00, 0x67, 0x00, 0x20, 0x00, 0x4c, 0x00, 0x69, 0x00, 0x6c,
    0x00, 0x20, 0x00, 0x57, 0x00, 0x61, 0x00, 0x79, 0x00, 0x6e, 0x00, 0x65,
    0x00, 0x20, 0x00, 0x26, 0x00, 0x20, 0x00, 0x42, 0x00, 0x75, 0x00, 0x73,
    0x00, 0x74, 0x00, 0x61, 0x00, 0x20, 0x00, 0x52, 0x00, 0x68, 0x00, 0x79,
    0x00, 0x6d, 0x00, 0x65, 0x00, 0x73, 0x00, 0x29, 0x00};

int main(int argc, char **argv) {
  puts("unicode main");
  char *out = NULL;
  size_t out_size = 0;
  utf16_to_ascii(tb, 105, &out, &out_size);
  fprintf(stderr, "Size: %zu, Text: %s\n", out_size, out);
  return EXIT_SUCCESS;
}
