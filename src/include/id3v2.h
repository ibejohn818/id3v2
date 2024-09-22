/*
id3v2_frame_picture_t *id3v2_frame_picture(id3v2_frame_t *f);
 * ID3v2 Ref: https://github.com/id3/ID3v2.4/blob/master/
 *
 */
#ifndef ID3V3_H
#define ID3V3_H

#include "id3v2/synch.h"
#include "id3v2/types.h"

#define ID3_HEADER 10
#define ID3_FRAME_HEADER 10
#define ID3_PADDING 2048

// flags

#define ID3_FLAG_UNSYNC_FIELDS 0b10000000
#define ID3_FLAG_EXTENDED_HEADER 0b01000000
#define ID3_FLAG_EXPERIMENTAL 0b00100000
#define ID3_FLAG_FOOTER 0b00010000

/**
 * load id3 tags by file
 */
id3v2_tag_t *id3v2_from_file(const char *file_name);

id3v2_tag_t *id3v2_from_buffer(const char *buf);

size_t id3v2_tag_eat_padding(id3v2_tag_t *t, size_t cursor_pos);
void id3v2_prepend_frame(id3v2_frame_list_t **head, id3v2_frame_t *f);

/**
 * parse the content of a text frame
 */
id3v2_frame_text_t *id3v2_frame_text(id3v2_frame_t *f);

/**
 * parse the content of a picture frame
 */
id3v2_frame_picture_t *id3v2_frame_picture(id3v2_frame_t *f);

/**
 *
 */
id3v2_frame_text_t *id3v2_frame_comment(id3v2_frame_t *f);

/**
 * write id3 tag w/music data to new file
 */
void id3v2_tag_write_file(id3v2_tag_t *t, const char *save_to);

/**
* Write the id3 tag to `buffer` and report its size to `size`.
* Function will allocate to `buffer`
*/
void id3v2_tag_write_to_buffer(id3v2_tag_t *t, unsigned char **buffer, size_t *size);

id3v2_frame_t *id3v2_tag_raw_frame_by_tag(id3v2_tag_t *t, const char tag[4]);

id3v2_frame_text_t *id3v2_frame_text_by_tag(id3v2_tag_t *t, const char tag[4]);

uint32_t id3v2_tag_total_frame_size(id3v2_tag_t *t);

void id3v2_tag_remove_frame_by_tag(id3v2_tag_t *t, const char tag[4]);
void id3v2_tag_remove_frame(id3v2_tag_t *t, id3v2_frame_t *f);

void id3v2_tag_free(id3v2_tag_t *t);
void id3v2_tag_free_text_frame(id3v2_frame_text_t *f);

// helper methods - read
id3v2_frame_text_t *id3v2_tag_title(id3v2_tag_t *t); // TIT2
id3v2_frame_text_t *id3v2_tag_album(id3v2_tag_t *t); // TALB
id3v2_frame_text_t *id3v2_tag_track(id3v2_tag_t *t); // TRCK
id3v2_frame_text_t *id3v2_tag_disc(id3v2_tag_t *t); // TPOS
id3v2_frame_text_t *id3v2_tag_artist(id3v2_tag_t *t); // TPE1

// helper methods - write / update
void id3v2_tag_write_text_frame(id3v2_tag_t *t, const char tag[4], const char *text);
void id3v2_tag_write_title(id3v2_tag_t *t, const char *text); // TIT2
void id3v2_tag_write_artist(id3v2_tag_t *t, const char *text); // TPE1
void id3v2_tag_write_album(id3v2_tag_t *t, const char *text); // TALB
void id3v2_tag_write_track(id3v2_tag_t *t, const char *text); // TRCK
void id3v2_tag_write_disc(id3v2_tag_t *t, const char *text); // TPOS



#endif
