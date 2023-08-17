#include "id3v2.h"
#include "id3v2/types.h"

id3v2_frame_text_t *id3v2_tag_artist(id3v2_tag_t *t) {
  return id3v2_frame_text_by_tag(t, "TPE1");
}

id3v2_frame_text_t *id3v2_tag_album(id3v2_tag_t *t) {
  return id3v2_frame_text_by_tag(t, "TALB");
}

id3v2_frame_text_t *id3v2_tag_title(id3v2_tag_t *t) {
  return id3v2_frame_text_by_tag(t, "TIT2");
}

id3v2_frame_text_t *id3v2_tag_track(id3v2_tag_t *t) {
  return id3v2_frame_text_by_tag(t, "TRCK");
}

id3v2_frame_text_t *id3v2_tag_disc(id3v2_tag_t *t) {
  return id3v2_frame_text_by_tag(t, "TPOS");
}

static id3v2_frame_t *create_raw_frame(id3v2_tag_t *t, const char tag[4], const char *buffer, size_t size) {

  id3v2_frame_t *f;
  f = (id3v2_frame_t *)malloc(sizeof(*f));

  memcpy(f->tag, tag, 4);
  // TODO: support frame flags
  f->flags[0] = 0;
  f->flags[1] = 0;
  f->size = size;
  f->buffer = (char *)buffer;

  return f;
}


void id3v2_tag_write_text_frame(id3v2_tag_t *t, const char tag[4], const char *text) {


  // add one to the length for the prefixed encoding byte
  size_t len = strlen(text) + 2;

  char *new_text = (char *)malloc(sizeof(char) * len);

  // TODO: add optional encoding (hard coded to latin1)
  *new_text = 0x00;

  // copy the given text
  memcpy(new_text + 1, text, (len-1));
  new_text[len] = '\0';

  // create the new frame
  id3v2_frame_t *new = create_raw_frame(t, tag, new_text, len);

  // check for existing frame (if present, remove will free it)
  id3v2_frame_t *f = id3v2_tag_raw_frame_by_tag(t, tag);

  if (f != NULL) {
    // frame exist, remove it
    id3v2_tag_remove_frame_by_tag(t, tag); 
  }

  // append to frames
  id3v2_prepend_frame(&t->frames, new);
}

void id3v2_tag_write_title(id3v2_tag_t *t, const char *text) {
  id3v2_tag_write_text_frame(t, "TIT2", text);
}

void id3v2_tag_write_artist(id3v2_tag_t *t, const char *text) {
  id3v2_tag_write_text_frame(t, "TPE1", text);
}

void id3v2_tag_write_album(id3v2_tag_t *t, const char *text) {
  id3v2_tag_write_text_frame(t, "TALB", text);
}

void id3v2_tag_write_track(id3v2_tag_t *t, const char *text) {
  id3v2_tag_write_text_frame(t, "TRCK", text);
}

void id3v2_tag_write_disc(id3v2_tag_t *t, const char *text) {
  id3v2_tag_write_text_frame(t, "TPOS", text);
}
