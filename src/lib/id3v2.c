#include "../include/id3v2.h"

static uint32_t int_decode(char *bytes, int size, int offset) {
  unsigned int result = 0x00;
  int i = 0;
  for (i = 0; i < size; i++) {
    result = result << 8;
    result = result | (unsigned char)bytes[offset + i];
  }

  return result;
}

static char *int_encode(int integer) {
  int i;
  int size = 4;
  char *result = (char *)malloc(sizeof(char) * size);

  // We need to reverse the bytes because Intel uses little endian.
  char *aux = (char *)&integer;
  for (i = size - 1; i >= 0; i--) {
    result[size - 1 - i] = aux[i];
  }

  return result;
}

static uint32_t synch_encode(int value) {
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

static uint32_t synch_decode(int value) {
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

id3v2_tag_t *jcid3v2_from_file(const char *file_name) {

  FILE *fd = fopen(file_name, "rb");

  id3v2_tag_t *t;
  t = (id3v2_tag_t *)malloc(sizeof(*t));

  // get the total size of the file
  fseek(fd, 0, SEEK_END);
  t->file_size = ftell(fd);
  rewind(fd);


  char header[ID3_HEADER];

  if (fread(&header, sizeof(char), ID3_HEADER, fd) != ID3_HEADER) {
    puts("unable to read id3 header");
    exit(1);
  }

  // check id3 tag marker
  if ((header[0] != 'I') || (header[1] != 'D') || (header[2] != '3')) {
    puts("invalid id3 header");
    exit(1);
  }

  // get major version byte
  t->version = header[3];

  // set flags
  t->flags.unsynch_fields = header[5] & 0b10000000;
  t->flags.extended_header = header[5] & 0b01000000;
  t->flags.expreimental = header[5] & 0b00100000;
  t->flags.footer = header[5] & 0b00010000;

  // decode the tag size
  t->tag_size = synch_decode(int_decode(header, 4, 6));

  printf("Tag size: %u \n", t->tag_size);

  fseek(fd, 10, SEEK_SET);

  // read tag buffer
  t->tag_buffer = (char *)calloc(t->tag_size, sizeof(char));
  fread(t->tag_buffer, sizeof(char), t->tag_size, fd);

  fclose(fd);

  return t;
}

id3v2_tag_t *jcid3v2_from_buffer(const char *buf) {
  id3v2_tag_t *t;
  t = (id3v2_tag_t *)malloc(sizeof(*t));

  return t;
}

/**
* validate a frame by checking its tag
*/
static bool _id3v2_validate_frame_tag(const char tag[4]) {
  // FIXME: tmp check during development


  for(uint8_t i = 0; i < 4; i++) {

    bool alpha_chk = tag[i] >= 'A' && tag[i] <= 'Z';
    bool numero_chk = tag[i] >= '0' && tag[i] <= '9';

    if (!alpha_chk && !numero_chk) {
      return false;
    }
  }

  return true;
}

size_t id3v2_tag_eat_padding(id3v2_tag_t *t, size_t cursor_pos) {

  size_t c = 0;
  size_t idx;
  while((cursor_pos + c) <= t->tag_size) {
    idx = (cursor_pos + c);

    char chk = t->tag_buffer[idx];
    // printf("chk: %s | tag size: %u \n", &chk, t->tag_size);
    if (chk != '\0') {
      return c;
    }
    c++;
  }

  return c;
}

id3v2_frame_t *id3v2_tag_parse_frame(id3v2_tag_t *t, size_t cursor_pos) {

  // validate the frame tag before we allocate the struct
  char tag[4];
  memcpy(&tag, (t->tag_buffer + cursor_pos), 4);

  // validate the tag and return NULL for invalid
  if (!_id3v2_validate_frame_tag(tag)) {
    return NULL;
  }

  // tag is valid, allocate the struct
  id3v2_frame_t *f;
  f = (id3v2_frame_t *)calloc(1, sizeof(*f));

  // copy tag to struct
  memcpy(&f->tag, tag, 4); 

  // move the cursor forward
  cursor_pos += 4;

  f->size = int_decode(t->tag_buffer, 4, cursor_pos);
  if(t->version == 4) {
    // version 4 gets the synch safe size
    f->size = synch_decode(f->size);
  }

  // move the cursor forward
  cursor_pos += 4;

  // get the flags
  memcpy(&f->flags, t->tag_buffer + cursor_pos, 2);

  cursor_pos += 2;

  // TODO: make content specific handlers (IE: text, apic, etc..)
  f->buf = (char *)calloc(f->size, sizeof(char)); 
  memcpy(f->buf, t->tag_buffer + cursor_pos, f->size); 

  cursor_pos += f->size;
  printf("func cursor pos: %lu \n", cursor_pos);

  return f;
}
