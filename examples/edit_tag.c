#include "../src/include/id3v2.h"

int main(int argc, char **argv) {

  if (argc < 2) {
    puts("argument 1 must point to an mp3 file");
    return -1;
  }

  char *file_name = argv[1];
  char *save_to = {0};

  if (argc >= 3) {
    save_to = argv[2];
  }

  id3v2_tag_t *tag = id3v2_from_file(file_name);
  if (tag == NULL) {
    puts("unable to parse file");
    return -2;
  }

  // pull up some text tags
  id3v2_frame_text_t *title = id3v2_tag_title(tag);
  // id3v2_frame_text_t *artist = id3v2_tag_artist(tag);
  // id3v2_frame_text_t *album = id3v2_tag_album(tag);
  // id3v2_frame_text_t *track = id3v2_tag_track(tag);

  printf("Title: %s \n", (title != NULL && title->text != NULL) ? title->text:"N/A");
  // printf("Artist: %s \n", (artist != NULL && artist->text != NULL) ? artist->text:"N/A");
  // printf("Album: %s \n", (album != NULL && album->text != NULL) ? album->text:"N/A");
  // printf("Track: %s \n", (track != NULL && track->text != NULL) ? track->text:"N/A");

  id3v2_tag_free_text_frame(title);
  // id3v2_tag_free_text_frame(artist);
  // id3v2_tag_free_text_frame(album);
  // id3v2_tag_free_text_frame(track);
  id3v2_tag_free(tag);

  return 0;
}
