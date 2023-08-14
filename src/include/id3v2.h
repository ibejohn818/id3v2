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

/**
 * load id3 tags by file
 */
id3v2_tag_t *id3v2_from_file(const char *file_name);
id3v2_tag_t *id3v2_from_buffer(const char *buf);

/**
 * Return the next frame from an id3v2_tag_t buffer.
 */
id3v2_frame_t *id3v2_tag_parse_frame(id3v2_tag_t *t, size_t *cursor_pos);
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

id3v2_frame_t *id3v2_tag_raw_frame_by_tag(id3v2_tag_t *t, const char tag[4]);

id3v2_frame_text_t *id3v2_frame_text_by_tag(id3v2_tag_t *t, const char tag[4]);

size_t id3v2_tag_total_frame_size(id3v2_tag_t *t);

// helper methods
id3v2_frame_text_t *id3v2_tag_title(id3v2_tag_t *t); // TIT2
id3v2_frame_text_t *id3v2_tag_album(id3v2_tag_t *t); // TALB
id3v2_frame_text_t *id3v2_tag_track(id3v2_tag_t *t); // TRCK
id3v2_frame_text_t *id3v2_tag_disc(id3v2_tag_t *t); // TPOS
id3v2_frame_text_t *id3v2_tag_artist(id3v2_tag_t *t); // TPE1

#endif
