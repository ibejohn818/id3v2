/*
 * ID3v2 Ref: https://github.com/id3/ID3v2.4/blob/master/
 *
 */
#ifndef _JHID3V3_H
#define _JHID3V3_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID3_HEADER 10
#define ID3_FRAME_HEADER 10

typedef struct {
  bool unsynch_fields;
  bool extended_header;
  bool expreimental;
  bool footer;
} id3v2_flags_t;

typedef struct {
  uint8_t version;
  uint32_t tag_size;
  size_t file_size;
  id3v2_flags_t flags;
  char *file_name;
  char *tag_buffer;

} id3v2_tag_t;

typedef enum {
  LATIN1,
  UTF16,
  UTF16BE,
  UTF8,
} id3v2_frame_encoding_t;

typedef struct {
  id3v2_frame_encoding_t encoding;
  char *text;
} id3v2_text_frame_t;

typedef struct {
  char tag[4];
  char flags[2];
  uint32_t size;
  char *buf;
  union {
    id3v2_text_frame_t *text;
  };
} id3v2_frame_t;

id3v2_tag_t *jcid3v2_from_file(const char *file_name);
id3v2_tag_t *jcid3v2_from_buffer(const char *buf);

/**
 * Return the next frame from an id3v2_tag_t buffer.
 */
id3v2_frame_t *id3v2_tag_parse_frame(id3v2_tag_t *t, size_t cursor_pos);
size_t id3v2_tag_eat_padding(id3v2_tag_t *t, size_t cursor_pos);

#endif
