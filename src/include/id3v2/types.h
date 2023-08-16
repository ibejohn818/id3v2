
#ifndef ID3V2_TYPES_H
#define ID3V2_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct id3v2_frame_list_s id3v2_frame_list_t;

typedef enum {
  LATIN1,
  UTF16,
  UTF16BE,
  UTF8,
} id3v2_frame_encoding_t;

typedef enum {
  OTHER = 0x00,
  PIXEL32,
  OTHER_ICON,
  FRONT_COVER,
  BACK_COVER,
  LEAFLET,
  MEDIA,
  LEAD_ARTIST,
  CONDUCTOR,
  BAND,
  COMPOSER,
  LYRICIST,
  RECORDING_LOC,
  DURING_RECORDING,
  DURING_PERFORMANCE,
  VIDEO_CAPTURE,
  COLORED_FISH,
  ILLUSTRATION,
  BAND_LOGO,
  PUBLISHER_LOGO,
} id3v2_picture_type_t;

/**
 * id3 tag flags
 */
typedef struct {
  bool unsynch_fields;
  bool extended_header;
  bool expreimental;
  bool footer;
} id3v2_flags_t;


/**
 * id3 tag.
 * tag_buffer holds bytes specified by tag_size
 */
typedef struct {
  uint8_t version;
  // TODO: finalize on a single flag solution
  id3v2_flags_t flags;
  uint32_t tag_size;
  char flag_byte;
  char version_bytes[2];

  // scanned file info
  char file_name[128];
  size_t file_size;

  // the entire tag buffer specified by 
  // tag_size minus the header
  char *tag_buffer;

  // linked list of raw frames
  id3v2_frame_list_t *frames;

  // mp3 data size
  size_t music_size;
  // mp3 file data
  char *music_data;

} id3v2_tag_t;


/**
 * raw id3 frame, use the following methods to extract content:
 * - id3v2_frame_text
 * - id3v2_frame_comment
 * - id3v2_frame_picture
 */
typedef struct {
  char tag[4];
  unsigned char flags[2];
  uint32_t size;
  char *buffer;
  // TODO: add unique id to handle edit/delete of dupes
} id3v2_frame_t;

typedef struct {
  id3v2_frame_t *frame;
  id3v2_frame_encoding_t encoding;
  char *text;
} id3v2_frame_text_t;

typedef struct {
  id3v2_frame_t *frame;
  id3v2_frame_encoding_t encoding;
  char *comment;
} id3v2_frame_comment_t;

typedef struct {
  id3v2_frame_t *frame;
  id3v2_frame_encoding_t encoding;
  char mime[32];
  id3v2_picture_type_t type;
  size_t size;
  unsigned char *buffer;
} id3v2_frame_picture_t;

typedef struct id3v2_frame_list_s {
  id3v2_frame_t *frame;
  struct id3v2_frame_list_s *next;
} id3v2_frame_list_t;


/*
typedef enum {
   AENC Audio encryption
   APIC Attached picture
   ASPI Audio seek point index

   COMM Comments
   COMR Commercial frame

   ENCR Encryption method registration
   EQU2 Equalisation (2)
   ETCO Event timing codes

   GEOB General encapsulated object
   GRID Group identification registration

   LINK Linked information

   MCDI Music CD identifier
   MLLT MPEG location lookup table

   OWNE Ownership frame

   PRIV Private frame
   PCNT Play counter
   POPM Popularimeter
   POSS Position synchronisation frame

   RBUF Recommended buffer size
   RVA2 Relative volume adjustment (2)
   RVRB Reverb

   SEEK Seek frame
   SIGN Signature frame
   SYLT Synchronised lyric/text
   SYTC Synchronised tempo codes

   TALB Album/Movie/Show title
   TBPM BPM (beats per minute)
   TCOM Composer
   TCON Content type
   TCOP Copyright message
   TDEN Encoding time
   TDLY Playlist delay
   TDOR Original release time
   TDRC Recording time
   TDRL Release time
   TDTG Tagging time
   TENC Encoded by
   TEXT Lyricist/Text writer
   TFLT File type
   TIPL Involved people list
   TIT1 Content group description
   TIT2 Title/songname/content description
   TIT3 Subtitle/Description refinement
   TKEY Initial key
   TLAN Language(s)
   TLEN Length
   TMCL Musician credits list
   TMED Media type
   TMOO Mood
   TOAL Original album/movie/show title
   TOFN Original filename
   TOLY Original lyricist(s)/text writer(s)
   TOPE Original artist(s)/performer(s)
   TOWN File owner/licensee
   TPE1 Lead performer(s)/Soloist(s)
   TPE2 Band/orchestra/accompaniment
   TPE3 Conductor/performer refinement
   TPE4 Interpreted, remixed, or otherwise modified by
   TPOS Part of a set
   TPRO Produced notice
   TPUB Publisher
   TRCK Track number/Position in set
   TRSN Internet radio station name
   TRSO Internet radio station owner
   TSOA Album sort order
   TSOP Performer sort order
   TSOT Title sort order
   TSRC ISRC (international standard recording code)
   TSSE Software/Hardware and settings used for encoding
   TSST Set subtitle
   TXXX User defined text information frame

   UFID Unique file identifier
   USER Terms of use
   USLT Unsynchronised lyric/text transcription

   WCOM Commercial information
   WCOP Copyright/Legal information
   WOAF Official audio file webpage
   WOAR Official artist/performer webpage
   WOAS Official audio source webpage
   WORS Official Internet radio station homepage
   WPAY Payment
   WPUB Publishers official webpage
   WXXX User defined URL link frame
} id3v2_frame_type;
*/

#endif
