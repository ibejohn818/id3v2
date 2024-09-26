#ifndef ID3V2_UNICODE_H
#define ID3V2_UNICODE_H

#include <stdio.h>

/**
* FIXME: hack for utf16 strings
*/
void utf16_to_ascii(const char *buf, size_t buf_size, char **out, size_t *out_size);


#endif // ID3V2_UNICODE_H

