#include "id3v2.h"
#include "id3v2/synch.h"
#include "id3v2/types.h"
#include "id3v2/unicode.h"

static id3v2_frame_list_t *scan_frames(id3v2_tag_t *t);

id3v2_tag_t *id3v2_from_file(const char *file_name) {

  FILE *fp = fopen(file_name, "rb");

  id3v2_tag_t *t = malloc(sizeof(*t));
  t->frames = NULL;

  memcpy(&t->file_name, file_name, strlen(file_name));

  // get the total size of the file
  fseek(fp, 0, SEEK_END);
  t->file_size = ftell(fp);
  rewind(fp);

  unsigned char header[ID3_HEADER];

  if (fread(&header, sizeof(char), ID3_HEADER, fp) != ID3_HEADER) {
    puts("unable to read id3 header");
    // exit(1);
    goto error;
  }

  // check id3 tag marker
  if ((header[0] != 'I') || (header[1] != 'D') || (header[2] != '3')) {
    puts("invalid id3 header");
    // exit(1);
    goto error;
  }

  // get major version_major byte
  t->version_major = header[3];
  t->version_minor = header[4];

  // set flags
  t->flag = header[5];

  // decode the tag size
  t->tag_size = synch_decode(int_decode(header, 4, 6));

  // fprintf(stderr, "Tag size: %u \n", t->tag_size);

  fseek(fp, 10, SEEK_SET);

  // read tag buffer
  t->tag_buffer = calloc(t->tag_size, sizeof(char));
  fread(t->tag_buffer, sizeof(char), t->tag_size, fp);

  // allocate music_data
  t->music_size = t->file_size - ftell(fp);
  t->music_data = malloc(sizeof(char) * t->music_size);

  // read data
  fread(t->music_data, sizeof(char), t->music_size, fp);

  // scan frames
  scan_frames(t);

  fclose(fp);

  return t;

error:
  if (t != NULL) {
    id3v2_tag_free(t);
  }
  fclose(fp);
  return NULL;
}

id3v2_tag_t *id3v2_from_buffer(const char *buf) {
  id3v2_tag_t *t;
  t = malloc(sizeof(*t));

  return t;
}

/**
 * validate a frame by checking its tag
 */
static bool _id3v2_validate_frame_tag(const char tag[4]) {
  // FIXME: tmp check during development

  for (uint8_t i = 0; i < 4; i++) {

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
  while ((cursor_pos + c) < t->tag_size) {
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

id3v2_frame_t *parse_frame(id3v2_tag_t *t, size_t *cursor_pos) {

  // validate the frame tag before we allocate the struct
  char tag[4];
  memcpy(&tag, (t->tag_buffer + *cursor_pos), 4);

  // validate the tag and return NULL for invalid
  if (!_id3v2_validate_frame_tag(tag)) {
    // fprintf(stderr, "Invalid frame tag: position: %lu Tag: %s \n", *cursor_pos, tag);
    return NULL;
  }

  // tag is valid, allocate the struct
  id3v2_frame_t *f = malloc(sizeof(*f));

  // copy tag to struct
  memcpy(&f->tag, tag, 4);

  // move the cursor forward
  *cursor_pos += 4;

  f->size = int_decode((unsigned char *)t->tag_buffer, 4, *cursor_pos);
  // fprintf(stderr, "frame size: %d \n", f->size);
  if (t->version_major == 4) {
    // version_major 4 gets the synch safe size
    f->size = synch_decode(f->size);
    fprintf(stderr, "sync safe frame: %d \n", f->size);
  }

  // move the cursor forward
  *cursor_pos += 4;

  // get the flags
  memcpy(&f->flags, t->tag_buffer + *cursor_pos, 2);
  *cursor_pos += 2;

  // if (strcmp(tag, "TIT2") == 0) {
  //   fprintf(stderr, "Found title \n");
  //   fprintf(stderr, "flags: %x %x\n", f->flags[0],  f->flags[1]);
  //   fprintf(stderr, "first bytes: %x %x\n", (t->tag_buffer + *cursor_pos)[0], (t->tag_buffer + *cursor_pos)[1]);
  //   for(size_t ii=0; ii < f->size; ii++) {
  //     fprintf(stderr, "0x%.2x\n", (t->tag_buffer + *cursor_pos)[ii]);
  //   }
  // }

  // allocate and copy the frame buffer
  // f->buffer = calloc(f->size + 1, sizeof(char));
  f->buffer = malloc(sizeof(char) * f->size);
  memcpy(f->buffer, t->tag_buffer + (*cursor_pos), (size_t)f->size);
 
  *cursor_pos += f->size;
  //  printf("func cursor pos: %lu \n", *cursor_pos);

  return f;
}

static id3v2_frame_list_t *scan_frames(id3v2_tag_t *t) {

  id3v2_frame_t *f = NULL;
  size_t cursor = 0;

  do {
    f = parse_frame(t, &cursor);
    if (f != NULL) {
      // check if this is going to be the head node
      if (t->frames == NULL) {
        id3v2_frame_list_t *l = malloc(sizeof(*l));
        t->frames = l;
        t->frames->frame = f;
        t->frames->next = NULL;
      } else {
        id3v2_prepend_frame(&t->frames, f);
      }
    }

  } while (f != NULL);

  return NULL;
}

void id3v2_prepend_frame(id3v2_frame_list_t **head, id3v2_frame_t *f) {

  id3v2_frame_list_t *l = malloc(sizeof(*l));

  // add incoming frame to new list node
  l->frame = f;
  // set new list element next to curren head frame
  l->next = *head;

  // replace the head with the new list node
  *head = l;
}

id3v2_frame_text_t *id3v2_frame_text(id3v2_frame_t *f) {

  // TODO: add options to automatically convert encoding

  if (f->tag[0] != 'T') {
    return NULL;
  }

  // alloc the text frame
  id3v2_frame_text_t *t = malloc(sizeof(*t));
  // t->frame = (id3v2_frame_t *)malloc(sizeof(*f));
  t->frame = f;

  // buffer position
  size_t cursor = 0;

  if (f->size <= 2) {
    t->text = NULL;
    t->encoding = LATIN1;
    return t;
  }

  // determine encoding
  // TODO: utf byte order marker?
  // FIXME: using a hack for utf16, we should make this 
  // more robust or use a library
  switch (f->buffer[cursor]) {
  case 0x00:
    t->encoding = LATIN1;
    break;
  case 0x01:
    t->encoding = UTF16;
    break;
  case 0x02:
    t->encoding = UTF16BE;
    break;
  case 0x03:
    t->encoding = UTF8;
    break;
  default:
    t->encoding = LATIN1;
  }
  cursor++;

  switch(t->encoding) {
  case UTF16:{
      char *out = NULL;
      size_t out_size = 0;
      utf16_to_ascii(f->buffer, f->size, &out, &out_size); 
      t->encoding = LATIN1;
      t->text = out;
      t->size = out_size;
      // fprintf(stderr, "Output: %s\n", t->text);
      break;
  }
  default: // LATIN1
    t->text = malloc(sizeof(char) * f->size);
    t->size = f->size;
    memcpy(t->text, f->buffer + cursor, f->size);
    break;
  }

  return t;
}

id3v2_frame_t *id3v2_tag_raw_frame_by_tag(id3v2_tag_t *t, const char tag[4]) {

  id3v2_frame_list_t *l = t->frames;

  do {
    if (strcmp(l->frame->tag, tag) == 0) {
      return l->frame;
    }

    l = l->next;

  } while (l != NULL);

  return NULL;
}

id3v2_frame_text_t *id3v2_frame_text_by_tag(id3v2_tag_t *t, const char tag[4]) {

  id3v2_frame_t *f = id3v2_tag_raw_frame_by_tag(t, tag);
  if (f == NULL) {
    return NULL;
  }

  id3v2_frame_text_t *txt = id3v2_frame_text(f);
  if (txt == NULL) {
    return NULL;
  }

  return txt;
}

static id3v2_picture_type_t picture_type(const char b) {
  switch (b) {
  case 0x00:
    return OTHER;
    break;
  case 0x01:
    return PIXEL32;
    break;
  case 0x02:
    return OTHER_ICON;
    break;
  case 0x03:
    return FRONT_COVER;
    break;
  case 0x04:
    return BACK_COVER;
    break;
  case 0x05:
    return LEAFLET;
    break;
  default:
    return OTHER;
  }
}

id3v2_frame_picture_t *id3v2_frame_picture(id3v2_frame_t *f) {

  if (f->tag[0] != 'A') {
    return NULL;
  }

  id3v2_frame_picture_t *p = malloc(sizeof(*p));
  p->frame = f;

  size_t cursor = 0;

  // determine encoding
  switch (f->buffer[cursor]) {
  case 0x00:
    p->encoding = LATIN1;
    break;
  case 0x01:
    p->encoding = UTF16;
    break;
  case 0x02:
    p->encoding = UTF16BE;
    break;
  case 0x03:
    p->encoding = UTF8;
    break;
  default:
    p->encoding = LATIN1;
  }

  cursor++;

  // get mime type
  size_t offset = 0;
  while (f->buffer[(cursor + offset)] != '\0') {
    printf("B: %c \n", f->buffer[(cursor + (offset))]);
    offset++;
  }
  memcpy(&p->mime, f->buffer + cursor, offset);

  cursor += offset;

  // determine picture type
  p->type = picture_type(f->buffer[cursor]);
  cursor++;

  // description
  offset = 0;
  while (f->buffer[(cursor + offset)] != '\0') {
    offset++;
  }
  // description should be at least 1 byte (null terminator)
  if (offset == 0) {
    offset++;
  }
  // TODO: copy description?
  cursor += offset;

  // remove any 0x00 bytes before image
  while (f->buffer[cursor] == 0x00) {
    cursor++;
  }

  // determine size of remaining bytes in buffer
  p->size = (f->size - cursor);
  printf("frame size: %u \n", f->size);
  printf("image size: %lu \n", p->size);
  // allocate and copy picture data
  p->buffer = calloc(p->size, sizeof(unsigned char));
  memcpy(p->buffer, f->buffer + cursor, p->size);

  return p;
}

uint32_t id3v2_tag_total_frame_size(id3v2_tag_t *t) {

  uint32_t total = 0;
  id3v2_frame_list_t *l = t->frames;
  while (l != NULL) {
    total += l->frame->size;
    total += 10;
    l = l->next;
  }
  return total;
}

static void write_header(id3v2_tag_t *t, unsigned char *buffer) {
  size_t p = 0;

  memcpy(buffer, "ID3", 3);
  p += 3;

  // version_major bytes
  memcpy(buffer + p, &t->version_major, 1);
  memcpy(buffer + p, &t->version_minor, 1);
  p += 2;

  // flag byte
  memcpy(buffer + p, &t->flag, 1);
  p += 1;

  // write synch safe int
  size_t total = id3v2_tag_total_frame_size(t);
  memcpy(buffer + p, int_encode(synch_encode(total)), 4);
}

static void write_frame_header(id3v2_tag_t *t, id3v2_frame_t *f,
                               unsigned char *buffer, size_t *pos) {

  // write tag
  memcpy(buffer + *pos, f->tag, 4);
  *pos += 4;

  // write the size;
  unsigned char *size;

  // synch int if version_major 4
  if (t->version_major == 4) {
    size = int_encode(synch_encode(f->size));
  } else {
    size = int_encode(f->size);
  }

  memcpy(buffer + *pos, size, 4);
  free(size);

  // memcpy(buffer + *pos, f->size_bytes, 4);
  *pos += 4;

  // write the flag bytes
  memcpy(buffer + *pos, f->flags, 2);
  *pos += 2;
}

void id3v2_tag_write_to_buffer(id3v2_tag_t *t, unsigned char **buffer,
                               size_t *size) {

  // start with the known header size
  size_t total = 10;
  total += id3v2_tag_total_frame_size(t);

  // add padding
  total += 2048;

  // allocate buffer
  unsigned char *b = calloc(total, sizeof(unsigned char));

  // write header
  write_header(t, b);

  // pointer pos
  size_t p = 10;

  // write frames
  id3v2_frame_list_t *l = t->frames;

  while (l != NULL) {

    id3v2_frame_t *f = l->frame;

    write_frame_header(t, f, b, &p);

    // write frame buffer
    memcpy(b + p, f->buffer, f->size);
    p += f->size;

    l = l->next;
  }

  *buffer = b;
  *size = total;
}

void id3v2_tag_remove_frame_by_tag(id3v2_tag_t *t, const char tag[4]) {

  if (t->frames == NULL) {
    return;
  }

  id3v2_frame_list_t *l = t->frames;
  id3v2_frame_list_t *prev = NULL;

  while (l != NULL) {

    if (strcmp(l->frame->tag, tag) == 0) {

      if (prev == NULL) {
        // if prev is empty, this is the head node
        // next becomes the new head
        t->frames = l->next;
      } else {
        // link this nodes next as the prev's next
        prev->next = l->next;
      }

      // free frame
      free(l->frame->buffer);
      free(l->frame);
      free(l);
      // exit loop
      break;
    }

    prev = l;
    l = l->next;
  }
}

void id3v2_tag_remove_frame(id3v2_tag_t *t, id3v2_frame_t *f) {

  if (t->frames == NULL) {
    return;
  }

  id3v2_frame_list_t *l = t->frames;
  id3v2_frame_list_t *prev = NULL;

  while (l != NULL) {

    if (l->frame == f) {

      if (prev == NULL) {
        // if prev is empty, this is the head node
        // next becomes the new head
        t->frames = l->next;
      } else {
        // link this nodes next as the prev's next
        prev->next = l->next;
      }

      // free frame
      free(l->frame->buffer);
      free(l->frame);
      free(l);
      // exit loop
      break;
    }

    prev = l;
    l = l->next;
  }
}

void id3v2_tag_free(id3v2_tag_t *t) {

  // free framesj
  id3v2_frame_list_t *l = t->frames;
  id3v2_frame_list_t *tmp;
  while (l != NULL) {

    tmp = l->next;
    free(l->frame->buffer);
    free(l->frame);
    free(l);
    l = tmp;
  }

  // free music data
  free(t->tag_buffer);
  free(t->music_data);
  free(t);
}

void id3v2_tag_free_text_frame(id3v2_frame_text_t *f) {
  free(f->text);
  free(f);
}
