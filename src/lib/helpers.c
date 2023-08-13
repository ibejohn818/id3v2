#include "id3v2.h"

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
