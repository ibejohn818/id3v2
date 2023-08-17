#include "../src/include/id3v2.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {

  if (argc < 2) {
    puts("argument 1 must point to a dir of mp3 files");
    return -1;
  }

  char *scan_path = argv[1];

  printf("Path: %s \n", scan_path);
  DIR *dp;
  if ((dp = opendir(scan_path)) == NULL) {
    puts("error opening directory");
    return -1;
  }

  struct dirent *dent;
  struct stat statbuf;

  chdir(scan_path);
  while ((dent = readdir(dp)) != NULL) {
    lstat(dent->d_name, &statbuf);
    if (S_ISDIR(statbuf.st_mode)) {
    } else {
      // printf("%s\n", dent->d_name);
      char mp3[255];
      sprintf(mp3, "%s/%s", scan_path, dent->d_name);
      printf("%s \n", mp3);
      // id3v2_tag_t *tag = id3v2_from_file(mp3);
      // if (tag == NULL) {
      //   puts("unable to parse file");
      //   return -2;
      // }
      // id3v2_frame_text_t *title = id3v2_tag_title(tag);
      // printf("Title: %s \n",
      //        (title != NULL && title->text != NULL) ? title->text : "N/A");
      // id3v2_tag_free_text_frame(title);
      // id3v2_tag_free(tag);
    }
  }

  // id3v2_tag_t *tag = id3v2_from_file(file_name);
  // if (tag == NULL) {
  //   puts("unable to parse file");
  //   return -2;
  // }
  //
  // // pull up some text tags
  // id3v2_frame_text_t *title = id3v2_tag_title(tag);
  // id3v2_frame_text_t *artist = id3v2_tag_artist(tag);
  // id3v2_frame_text_t *album = id3v2_tag_album(tag);
  // id3v2_frame_text_t *track = id3v2_tag_track(tag);
  //
  // printf("Title: %s \n", (title != NULL && title->text != NULL) ?
  // title->text:"N/A"); printf("Artist: %s \n", (artist != NULL &&
  // artist->text != NULL) ? artist->text:"N/A"); printf("Album: %s \n",
  // (album != NULL && album->text != NULL) ? album->text:"N/A");
  // printf("Track: %s \n", (track != NULL && track->text != NULL) ?
  // track->text:"N/A");

  // id3v2_tag_free_text_frame(title);
  // id3v2_tag_free_text_frame(artist);
  // id3v2_tag_free_text_frame(album);
  // id3v2_tag_free_text_frame(track);
  // id3v2_tag_free(tag);

  return 0;
}
