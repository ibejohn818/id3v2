#include "id3v2.h"
#include "id3v2/types.h"

static id3v2_frame_list_t *scan_frames(id3v2_tag_t *t);

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

  // scan frames
  scan_frames(t);

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

id3v2_frame_t *id3v2_tag_parse_frame(id3v2_tag_t *t, size_t *cursor_pos) {

  // validate the frame tag before we allocate the struct
  char tag[4];
  memcpy(&tag, (t->tag_buffer + *cursor_pos), 4);

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
  *cursor_pos += 4;

  f->size = int_decode(t->tag_buffer, 4, *cursor_pos);
  if (t->version == 4) {
    // version 4 gets the synch safe size
    f->size = synch_decode(f->size);
  }

  // move the cursor forward
  *cursor_pos += 4;

  // get the flags
  memcpy(&f->flags, t->tag_buffer + *cursor_pos, 2);

  *cursor_pos += 2;

  // allocate and copy the frame buffer
  f->buffer = (char *)calloc(f->size, sizeof(char));
  memcpy(f->buffer, t->tag_buffer + *cursor_pos, f->size);

  *cursor_pos += f->size;
  //  printf("func cursor pos: %lu \n", *cursor_pos);

  return f;
}

static id3v2_frame_list_t *scan_frames(id3v2_tag_t *t) {

  id3v2_frame_t *f = NULL;
  size_t cursor = 0;

  do {
    // TODO: eat possible padding between frames
    f = id3v2_tag_parse_frame(t, &cursor);
    if (f != NULL) {
      // check if this is going to be the head node
      if (t->frames == NULL) {
        t->frames = (id3v2_frame_list_t *)malloc(sizeof(id3v2_frame_list_t *));
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

  id3v2_frame_list_t *l;
  l = (id3v2_frame_list_t *)malloc(sizeof(*l));

  // add incoming frame to new list node
  l->frame = f;
  // set new list element next to curren head frame
  l->next = *head;

  // replace the head with the new list node
  *head = l;
}

id3v2_frame_text_t *id3v2_frame_text(id3v2_frame_t *f) {
  if (f->tag[0] != 'T') {
    return NULL;
  }

  // alloc the text frame
  id3v2_frame_text_t *t;
  t = (id3v2_frame_text_t *)malloc(sizeof(*t));
  t->frame = (id3v2_frame_t *)malloc(sizeof(*f));
  t->frame = f;

  // buffer position
  size_t cursor = 0;
  // size of the null terminator to apply
  size_t null_term_size = 1;

  // determine encoding
  // TODO: utf byte order marker?
  switch(f->buffer[cursor++]) {
    case 0x00:
      t->encoding = LATIN1;
      break;
    case 0x01:
      t->encoding = UTF16;
      null_term_size = 2;
      break;
    case 0x02:
      t->encoding = UTF16BE;
      null_term_size = 2;
      break;
    case 0x03:
      t->encoding = UTF8;
      null_term_size = 2;
      break;
    default:
      printf("Illegal encoding for frame: %s \n", f->tag);
      puts("Defaulting to LATIN1");
      t->encoding = LATIN1;
  } 
  
  t->text = (char *)calloc((f->size + null_term_size), sizeof(char));
  memcpy(t->text, f->buffer + cursor, f->size);

  return t;
}

id3v2_frame_t *id3v2_tag_raw_frame_by_tag(id3v2_tag_t *t, const char tag[4]) {

  id3v2_frame_list_t *l = t->frames;

  do {
    if (strcmp(l->frame->tag, tag) == 0) {
      return l->frame;
    }

    l = l->next;

  } while(l != NULL);


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
