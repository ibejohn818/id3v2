#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include "../include/jhid3v2.h"



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


int main(int argc, char **argv) {

  const char file_name[] = "/tmp/song4.mp3";

  id3v2_tag_t *tag = jcid3v2_from_file(file_name);

  id3v2_frame_t *f = id3v2_tag_parse_frame(tag, 0);

  if ((strcmp("TENC", f->tag)) == 0) {
    puts("Tag matched!");
  }

  printf("\n\n");
  for(uint32_t i=0; i < f->size; i++) {
    printf("0x%x ", f->buf[i]);
  }
  printf("\n\n");

  puts("hello world");
  return 0;
}
