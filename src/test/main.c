#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <uchar.h>
#include <iconv.h>

#include "../include/id3v2.h"

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

void write_image(id3v2_frame_picture_t *f) {

  FILE *fd = fopen("/tmp/test.jpg", "w");
  if (fd <= 0) {
    puts("error creating file");
    return;
  }

  size_t bytes_written = fwrite(f->buffer, 1, f->size, fd);
  printf("Bytes written: %lu \n", bytes_written); 

  fclose(fd);

}

int main(int argc, char **argv) {
  // const char file_name[] = "/tmp/song1.mp3";
  const char *file_name = argv[1];

  id3v2_tag_t *tag = id3v2_from_file(file_name);
  printf("ID3 Version: %u \n", tag->version);

  id3v2_frame_list_t *l = tag->frames;

  while (l != NULL) {
    printf("Frame: %s | size: %u \n", l->frame->tag, l->frame->size);
    l = l->next;
  }

  iconv_t conv = iconv_open("UTF-8", "UTF-16LE");

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

  tt = id3v2_tag_title(tag);
  if (tt != NULL) {
    printf("Title: %s \n", tt->text);
    // size_t conv_size = 128;
    // char *conv_buff = (char *)malloc(sizeof(char) * conv_size);
    // iconv(conv, (char **)&tt->frame->buffer, (size_t *)&tt->frame->size, (char **)&conv_buff, &conv_size);
    // printf("Frame Size: %lu | Conv: %s \n", (size_t)tt->frame->size, conv_buff);
    // char *ubuff = (char *)calloc(65, sizeof(char));
    // size_t ubuff_size = 65;
    // iconv(conv, &tt->text, (size_t *)&tt->frame->size - 4, &ubuff, &ubuff_size);
  }

  id3v2_frame_t *pr = id3v2_tag_raw_frame_by_tag(tag, "APIC");
  id3v2_frame_picture_t *pic = NULL;
  if (pr != NULL) {
    pic = id3v2_frame_picture(pr);
  }

  if (pic != NULL) {
    write_image(pic);
  }

  tt = id3v2_tag_title(tag);
  for(size_t i=0; i < tt->frame->size; i++) {
    printf("B: %x | ", tt->frame->buffer[i]);
  }
  printf("\n");

  // for(size_t i=0; i < pic->frame->size; i++) {
  // for(size_t i=0; i < 30; i++) {
  //   printf("B: %x | ", pic->buffer[i]);
  // }
  // printf("\n");

  printf("File name: %s \n", tag->file_name);
  size_t total = 0;

  l = tag->frames;
  while(l != NULL) {
    total += l->frame->size;
    l = l->next;
  }

  printf("Open-Total: %u | Calc-Total: %lu \n", tag->tag_size, total);

  // id3v2_tag_write_file(tag, "/tmp/test-2.mp3");

  puts("here");

  return 0;
}
