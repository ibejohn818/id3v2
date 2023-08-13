#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../include/id3v2.h"

static bool validate(const char tag[4]) {
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

char *readFile(const char *fileName) {

  FILE *fp = fopen(fileName, "r");

  if (fp == NULL) {
    printf("error opening file: %s\n", fileName);
    exit(1);
  }

  // get total bytes of file
  fseek(fp, 0L, SEEK_END);
  size_t fileSize = ftell(fp);
  rewind(fp);

  // TODO: debug filesize
  printf("Size: %zu\n", fileSize);

  // create buffer + 1 to ensure EOF (null byte)
  char *buffer = malloc((fileSize * sizeof(char)) + 1);
  if (buffer == NULL) {
    puts("unable to allocate buffer");
    exit(1);
  }

  // read file into buffer
  size_t bytesRead = fread(buffer, sizeof(char), fileSize, fp);
  if (bytesRead < fileSize) {
    puts("unable to read whole file into buffer \n");
    exit(1);
  }

  printf("Bytes read: %zu \n", bytesRead);

  // append null-byte
  buffer[bytesRead] = '\0';

  fclose(fp);

  return buffer;
}

void print_bytes(char *buf, size_t start, size_t len) {

  for (size_t i = start; i < len; i++) {
    // printf("0x%X ", (uint8_t)buffer[i]);
    printf("%c ", buf[i]);
  }
  puts("\n");
}

int main(int argc, char **argv) {
  // const char file_name[] = "/tmp/song1.mp3";
  const char *file_name = argv[1];

  id3v2_tag_t *tag = jcid3v2_from_file(file_name);
  printf("ID3 Version: %u \n", tag->version);

  id3v2_frame_list_t *l = tag->frames;

  while(l != NULL) {
    printf("Frame: %s | size: %u \n", l->frame->tag, l->frame->size);
    l = l->next;
  }

  id3v2_frame_text_t *tt;

  tt = id3v2_tag_artist(tag);
  if (tt != NULL) {
    printf("Artist: %s \n", tt->text);
  }

  tt = id3v2_tag_album(tag);
  if (tt != NULL) {
    printf("Album: %s \n", tt->text);
  }

  tt = id3v2_tag_track(tag);
  if (tt != NULL) {
    printf("Track: %s \n", tt->text);
  }
  puts("here"); 

  return 0;
}
