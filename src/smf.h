#ifndef SMF_H
#define SMF_H

#include <stdio.h>
#include <gdk/gdk.h>

struct smf_struct {
	FILE		*stream;
	void		*file_buffer;
	int		file_buffer_length;

	int		next_chunk_offset;

	int		format;
	int		number_of_tracks;

	/* These fields are extracted from "division" field of MThd header.  Valid is _either_ ppqn or frames_per_second/resolution. */
	int		ppqn;
	int		frames_per_second;
	int		resolution;
	int		microseconds_per_quarter_note;

	GQueue		*tracks_queue;
	int		last_track_number;
};

typedef struct smf_struct smf_t;

struct smf_track_struct {
	smf_t		*smf;

	void		*file_buffer;
	int		file_buffer_length;

	int		track_number;

	int		next_event_offset; /* Offset into buffer, used in parse_next_event(). */
	int		next_event_number;
	int		last_status; /* Used for "running status". */

	GQueue		*events_queue;
	int		time_of_next_event; /* Absolute time of next event on events_queue. */
};

typedef struct smf_track_struct smf_track_t;

struct smf_event_struct {
	smf_track_t	*track;

	int		time;
	int		track_number;
	unsigned char	*midi_buffer;
	int		midi_buffer_length; /* Length of the MIDI message in the buffer, in bytes. */
};

typedef struct smf_event_struct smf_event_t;

smf_t *smf_load(const char *file_name);
int smf_get_number_of_tracks(smf_t *smf);
smf_event_t *smf_get_next_event(smf_t *smf);
smf_event_t *smf_peek_next_event(smf_t *smf);
int smf_seek_to(smf_t *smf, double seconds);
double smf_event_time(const smf_event_t *event);
int event_is_metadata(const smf_event_t *event);

/* These are private. */
smf_t *smf_new(void);
void smf_free(smf_t *smf);
smf_track_t *smf_track_new(smf_t *smf);
void smf_track_free(smf_track_t *track);
smf_event_t *smf_event_new(smf_track_t *track);
void smf_event_free(smf_event_t *event);

char *smf_string_from_event(const smf_event_t *event);

#endif /* SMF_H */

