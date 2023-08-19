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
  struct dirent *ep;
  struct stat statbuf;

  dp = opendir (scan_path);
  if (dp != NULL)
    {
      while ((ep = readdir (dp))) {
        char file_path[255] = {0};
        sprintf(file_path, "%s/%s", scan_path, ep->d_name);
        if(lstat(file_path, &statbuf) == -1) {
          printf("lstat error \n");
          continue;
        }

           printf("File type:                ");

           switch (statbuf.st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device ");            break;
           case S_IFCHR:  printf("character device ");        break;
           case S_IFIFO:  printf("FIFO/pipe ");               break;
           case S_IFLNK:  printf("symlink ");                 break;
           case S_IFREG:  printf("regular file ");            break;
           case S_IFDIR:  printf("directory ");               break;
           case S_IFSOCK: printf("socket ");                  break;
           default:       printf("unknown? ");                break;
           }
        puts (ep->d_name);
      }
      (void) closedir (dp);
    }
  else
    perror ("Couldn't open the directory");


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
