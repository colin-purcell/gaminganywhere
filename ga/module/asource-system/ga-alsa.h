/*
 * Copyright (c) 2013-2014 Chun-Ying Huang
 *
 * This file is part of GamingAnywhere (GA).
 *
 * GA is free software; you can redistribute it and/or modify it
 * under the terms of the 3-clause BSD License as published by the
 * Free Software Foundation: http://directory.fsf.org/wiki/License:BSD_3Clause
 *
 * GA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You should have received a copy of the 3-clause BSD License along with GA;
 * if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __XCAP_ALSA_H__
#define	__XCAP_ALSA_H__

#include <sys/types.h>
#include <alsa/asoundlib.h>

#include <list>

struct Xcap_alsa_param {
	snd_pcm_t *handle;
	snd_output_t *sndlog;
	snd_pcm_format_t format;
	unsigned int channels;
	unsigned int samplerate;
	snd_pcm_uframes_t chunk_size;	// # of frames in a read
	snd_pcm_uframes_t buffer_size;
	size_t bits_per_sample;		// S16LE = 16-bits
	size_t bits_per_frame;		// bits_per_sample * # of channels
	size_t chunk_bytes;		// chunk_size * bits_per_frame
};

class XcapAlsaReadiQueue {
private:
	int n;
	int chunksize;			// max chunk size
	int chunkbyte;			// unitsize per chunk
	unsigned char *buffer;
	// pool data format: ret value (sizeof(snd_pcm_sframes_t)) + data (up to chunksize)
	std::list<unsigned char*> freepool;
	std::list<unsigned char*> datapool;
public:
	XcapAlsaReadiQueue(int n, int chunksize, int chunkbyte);
	~XcapAlsaReadiQueue();
	snd_pcm_sframes_t readi(snd_pcm_t *handle, unsigned char *samples, int chunksize, int queue);
	unsigned int size();
};

int ga_alsa_set_param(struct Xcap_alsa_param *param);
snd_pcm_t* ga_alsa_init(snd_output_t **pout);
void ga_alsa_close(snd_pcm_t *handle, snd_output_t *pout);

#endif
