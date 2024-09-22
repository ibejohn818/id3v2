#include <iconv.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

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

  FILE *fd = fopen("/tmp/test.jpg", "wb");
  if (fd <= 0) {
    puts("error creating file");
    return;
  }

  size_t bytes_written = fwrite(f->buffer, f->size, 1, fd);
  printf("Bytes written: %lu \n", bytes_written);

  fclose(fd);
}

void write_id3_buffer(id3v2_tag_t *t, const char *save_to,
                      unsigned char *buffer, size_t buffer_size) {

  FILE *fp = fopen(save_to, "w");

  // write tag
  fwrite(buffer, sizeof(char), buffer_size, fp);

  // write music data
  fwrite(t->music_data, sizeof(char), t->music_size, fp);

  fclose(fp);
}

void utf16_to_ascii(const unsigned char *utf16_buf, size_t utf16_size,
                    unsigned char **buf, size_t *size) {

  // check if there is a BOM, if so we skip
  bool bom_le = utf16_buf[0] == 0xff && utf16_buf[1] == 0xfe;
  bool bom_be = utf16_buf[0] == 0xfe && utf16_buf[1] == 0xff;

  size_t upos = 0;

  // if bom is present,
  // move starting position up 2 bytes
  if (bom_be || bom_le) {
    upos += 2;
  }

  unsigned char *abuf = (unsigned char *)calloc(utf16_size, sizeof(char));
  // save the abuf pointers starting address to transfer over to *buf
  unsigned char *abuf_start = abuf;

  // walk the utf16 byte pairs and check if the first
  // byte is valid ascii and append to the *abuf pointer
  while (upos < utf16_size) {
    // check if we're at the last byte
    if ((upos + 1) == utf16_size) {
      // add null terminator and break out
      *abuf = '\0';
      *size += 1;
      break;
    }
    // byte to check for ascii
    unsigned char chk = utf16_buf[upos];
    // next byte should be 0x00
    if (utf16_buf[upos + 1] != 0x00 || chk & 0x80 || chk > 128) {
      // invalid utf16 pair, skip
      // TODO: fill in acsii with a default char?
      // printf("Skipping utf16 pair @%lu \n", upos);
      upos += 2;
      continue;
    }

    *abuf = chk;
    // printf("Adding ascii: %c\n", *abuf);
    abuf++;
    upos += 2;
    *size += 1;
  }

  *buf = abuf_start;
}

int main(int argc, char **argv) {
  // const char file_name[] = "/tmp/song1.mp3";
  char file_name[64] = {0};
  char save_to[64] = {0};

  memcpy(file_name, argv[1], strlen(argv[1]));

  if (argc >= 3) {
    memcpy(save_to, argv[2], strlen(argv[2]));
  }
  // file_name = argv[1];

  id3v2_tag_t *tag = id3v2_from_file(file_name);
  printf("ID3 Version: %u \n", tag->version_major);

  uint32_t calc_size = id3v2_tag_total_frame_size(tag);
  printf("Calc size: %u\n", calc_size);

  id3v2_frame_list_t *l = tag->frames;

  while (l != NULL) {
    printf("Frame: %s | size: %u \n", l->frame->tag, l->frame->size);
    l = l->next;
  }

  id3v2_frame_text_t *tt;

  id3v2_tag_write_artist(tag, "Super Sublime");
  tt = id3v2_tag_artist(tag);
  if (tt != NULL) {
    printf("Artist: %s \n", tt->text);
    id3v2_tag_free_text_frame(tt);
  }

  tt = id3v2_tag_album(tag);
  if (tt != NULL) {
    printf("Album: %s \n", tt->text);
    id3v2_tag_free_text_frame(tt);
  }

  tt = id3v2_tag_track(tag);
  if (tt != NULL) {
    printf("Track: %s \n", tt->text);
    id3v2_tag_free_text_frame(tt);
  }

  // update title
  id3v2_tag_write_title(tag, "an updated title");
  tt = id3v2_tag_title(tag);
  if (tt != NULL) {
    printf("Title: %s \n", tt->text);
    id3v2_tag_free_text_frame(tt);
  }
  /*
  tt = id3v2_tag_title(tag);
  if (tt != NULL) {
    // printf("Title: %s \n", tt->text);

    unsigned char *out_buf;
    size_t out_size = 0;
    utf16_to_ascii((unsigned char *)tt->frame->buffer + 1, tt->frame->size,
                   &out_buf, &out_size);
    printf("Size: %lu, Title: %s \n", out_size, out_buf);

  }
  */

  printf("File name: %s \n", tag->file_name);

  if (strlen(save_to) > 0) {
    // id3v2_tag_write_file(tag, "/tmp/test.mp3");
    unsigned char *save_buffer;
    size_t save_size;

    id3v2_tag_write_to_buffer(tag, &save_buffer, &save_size);

    // write buffer to file
    write_id3_buffer(tag, save_to, save_buffer, save_size);
    // for (uint32_t i = 0; i < save_size; i++) {
    //   printf("C: %c \n", save_buffer[i]);
    // }
    free(save_buffer);
  }

  id3v2_tag_free(tag);

  puts("here");

  return 0;
}
